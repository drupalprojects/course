<?php

/**
 * Parent abstract base class of all course objects.
 *
 * Represents a course object in the database.
 *
 * Also holds a fulfillment record if a user is given.
 */
class CourseObject {
  // Hold requirement/object from database and the fulfillment object.
  private $requirement = array();
  private $courseObjectFulfillment = stdClass;

  /**
   * Construct a course object.
   *
   * Constructing with a requirement array will pre-load this object. This is
   * useful for query saving when constructing many objects at once is
   * necessary. Passing a numeric instance ID will load that requirement from
   * the database in the context of the course. Providing the user will either
   * load an existing fulfillment from the database or create a new fulfillment
   * object.
   *
   * @param array $requirement
   * @param object $user
   */
  function __construct($requirement, $user = NULL) {
    if (is_numeric($requirement) && $requirement > 0) {
      $sql = "select * from {course_outline} where instance = %d";
      $this->requirement = db_fetch_object(db_query($sql, $requirement));
    }
    else {
      $this->requirement = $requirement;
    }

    if ($user) {
      $this->courseObjectFulfillment = new CourseObjectFulfillment($this, $user);
    }
  }

  /**
   * Get the URL to take this course object, if any.
   *
   * @return string
   */
  public function getTakeUrl() {}

  /**
   * Get the URL to edit this course object, if any.
   *
   * @return string
   */
  public function getEditUrl() {}

  /**
   * If graded, grade this course object against a user.
   */
  public function grade($user = NULL) {}

  /**
   * Is this course object graded?
   *
   * @return bool
   */
  public function isGraded() {
    return $this->requirement->graded;
  }

  /**
   * Is this course object required for course completion?
   *
   * @return bool
   */
  public function isRequired() {
    return $this->requirement->required;
  }

  /**
   * Get the passing grade for this course object.
   *
   * @return float
   */
  public function getPassingGrade() {
    return $this->requirement->passing_grade;
  }

  /**
   * Set passing grade for this course object.
   *
   * @param float $grade
   *   Percentage to pass.
   */
  public function setPassingGrade($grade) {
    $this->requirement->passing_grade = $grade;
    return $this;
  }

  /**
   * Set requirement graded or not.
   *
   * @param bool $graded
   *    Graded, true or false.
   */
  public function setGraded($graded) {
    $this->requirement->graded = $graded;
    return $this;
  }

  /**
   * Get the title of this course object.
   *
   * @return string
   */
  public function getTitle() {
    return $this->requirement->title;
  }

  /**
   * Get the status message of this course object.
   */
  public function getStatus($type = 'short') {
    /*
     * if ($this->courseObjectFulfillment->isComplete()) {
     *   // do other messages?
     * }
     * else {
     *  if ($type == 'short') {
     *  return "Not viewed";
     * }
     *
     * if ($type == 'long') {
     * return "You have not viewed $title";
     * }
     * }
     */
  }

  /**
   * Get the course object ID of this course object.
   *
   * @return int
   */
  public function getId() {
    return $this->requirement->snid;
  }

  /**
   * Get this course object's fulfillment object.
   *
   * @return CourseObjectFulfillment
   */
  public function getFulfillment() {
    return $this->courseObjectFulfillment;
  }

  /**
   * Get the instance ID. This could be the external component ID, a Node ID...
   *
   * @return int
   */
  function getInstanceId() {
    return $this->requirement->instance;
  }

  /**
   * Get the course node ID this object belongs to.
   */
  function getCourseNid() {
    return $this->requirement->nid;
  }

  /**
   * Save the course object to the database.
   */
  public function save() {
    $keys = $this->requirement->snid ? array('snid') : array();
    return drupal_write_record('course_outline', $this->requirement, $keys);
  }
}

/**
 * Parent class for course object fulfillment.
 *
 * Represents the fulfillment record in the database.
 *
 */
class CourseObjectFulfillment {
  // Fulfillment array.
  private $fulfillment = array();
  private $courseObject;

  /**
   * Construct the fulfillment object.
   *
   * A CourseObject and user are required to construct a fulfillment object.
   *
   * @param CourseObject $courseObject
   * @param Object $user
   */
  function __construct($courseObject, $user) {
    $this->courseObject = $courseObject;
    $sql = "select * from {course_outline_fulfillment} where snid = %d and uid = %d";
    $this->fulfillment = db_fetch_array(db_query($sql, $courseObject->getId(), $user->uid));

    // Set requirement ID.
    $this->fulfillment['snid'] = $courseObject->getId();
    $this->fulfillment['uid'] = $user->uid;
  }

  /**
   * Is this fulfillment complete?
   *
   * @return bool
   */
  function isComplete() {
    return (bool) $this->fulfillment['complete'];
  }

  /**
   * Set this fulfillment complete.
   *
   * @param bool $complete
   *   Set to 0 to un-complete, 1 or omit to complete.
   */
  function setComplete($complete = 1) {
    $this->fulfillment['complete'] = $complete;
    return $this;
  }

  /**
   * Set this fulfillment's grade.
   *
   * @param float $grade
   */
  function setGrade($grade) {
    $this->fulfillment['grade_result'] = $grade;
    return $this;
  }

  /**
   * Get this fulfillment's grade.
   *
   * @return float
   *   A float value of the user's grade for this fulfillment.
   */
  function getGrade() {
    return $this->fulfillment['grade_result'];
  }

  /**
   * Get this fulfillment's course object.
   */
  function getCourseObject() {
    return $this->courseObject;
  }

  /**
   * Save this fulfillment to the database.
   */
  function save() {
    $keys = $this->fulfillment['sfid'] ? array('sfid') : array();
    $ret = drupal_write_record('course_outline_fulfillment', $this->fulfillment, $keys);

    // Create a Course to track the user's progress.
    $course = new Course($this->getCourseObject()->getCourseNid(), $this->fulfillment['uid']);
    $course->track();

    return $ret;
  }
}

/**
 * A course object that uses a node as a base.
 */
class CourseObjectNode extends CourseObject {
  protected $node;

  public function __construct($requirement, $user = NULL) {
    parent::__construct($requirement, $user);
    $this->node = node_load($this->getInstanceId());
  }

  public function getTakeUrl() {
    return url("node/{$this->node->nid}");
  }

  public function getEditUrl() {
    return url("node/{$this->node->nid}/edit");
  }
}
