<?php

/**
 * @file course.core.inc
 * File for main Course class.
 */
class CourseHandler {

  // Could be 'outline', 'course_object', 'settings'...
  public $handlerType = NULL;
  // For objects that store things in the database, this is the field where all
  // non-schema fields will be serialized to.
  public $table = NULL;
  public $primaryKey = NULL;
  public $serializedField = NULL;
  // Configuration for this handler.
  protected $config = array();
  private $accessMessages = array();

  function __construct($config = array()) {
    foreach ($config as $key => $value) {
      if ($key === $this->serializedField && !is_array($value)) {
        $data = unserialize($value);
        if (is_array($data)) {
          foreach ($data as $key2 => $value2) {
            $this->setOption($key2, $value2);
          }
        }
      }
      else {
        $this->setOptions((array) $config);
      }
    }
  }

  /**
   * Stub. Get the summary of an object's options.
   *
   * @return array
   *   An associative array of summary keys and values.
   */
  public function getOptionsSummary() {
    return array();
  }

  /**
   * Get an object's configuration.
   *
   * This can be overridden. For example, values stored in courseobject sessions
   * need to have priority over those in the database.
   *
   * @return array
   */
  public function getOptions() {
    return array_merge($this->optionsDefinition(), (array) $this->config);
  }

  /**
   * Get an option stored in this CourseObject.
   *
   * @return mixed
   */
  public final function getOption($key) {
    $config = $this->getOptions();
    return $config[$key];
  }

  /**
   * Set an option for this handler.
   *
   * @param $option string
   *   An option key.
   * @param $value mixed
   *   The option value.
   *
   * @return CourseHandler
   */
  public function setOption($option, $value) {
    $this->config[$option] = $value;
    return $this;
  }

  /**
   * Set this entire handler's options.
   *
   * Deserialize the serialized column if necessary.
   *
   * @param $options array
   *   An array of options.
   *
   * @return CourseHandler
   */
  protected final function setOptions($options) {
    $config = (array) $options;
    // Make sure the serialized field is not already extracted.
    if (is_string($config[$this->serializedField])) {
      $data = unserialize($config[$this->serializedField]);
      if (is_array($data)) {
        // Merge serialized data onto options. Schema fields take precedence.
        $config = array_merge($data, $config);
      }
    }
    $this->config = $config;
    return $this;
  }

  /**
   * Handlers need to declare their defaults if they have a configuration form.
   */
  protected function optionsDefinition() {
    $options = array();

    return $options;
  }

  /**
   * Handlers can declare a form.
   */
  public function optionsForm(&$form, &$form_state) {

  }

  /**
   * Validate?
   */
  public function optionsValidate(&$form, &$form_state) {

  }

  /**
   * Save data somewhere.
   *
   * This can be overridden. For example, values stored in courseobject sessions
   * need to have priority over those in the database.
   */
  public function optionsSubmit(&$form, &$form_state) {

  }

  /**
   * Return an array of database fields. This determines what fields should be
   * serialized instead of stored.
   */
  protected function getDatabaseFields() {
    $schema = drupal_get_schema($this->table);
    return array_keys($schema['fields']);
  }

  /**
   * Return a list of warning strings about this handler.
   *
   * For example, if a user adds a quiz to a course with no questions, trigger a
   * message.
   *
   * @see CourseObjectQuiz
   * @see CourseObjectWebform
   */
  public function getWarnings() {
    return array();
  }

  /**
   * Set an access message to be displayed along with the course object when it
   * is in the outline. For example, "This activity will open on XYZ" or "Please
   * complete Step 1 to take this activity."
   *
   * @param string $key
   *   Message key.
   * @param string $message
   *   Message text.
   */
  public function setAccessMessage($key = NULL, $message = NULL) {
    if ($key == NULL) {
      return $this->accessMessages;
    }
    elseif ($message != NULL) {
      $this->accessMessages[$key] = $message;
    }
  }

  /**
   * Get an array of access messages.
   *
   * @return array
   */
  public function getAccessMessages() {
    return $this->setAccessMessage();
  }

  public function save() {
    $options = $this->getOptions();
    if (!$options['uuid']) {
      $options['uuid'] = uuid_uuid();
    }

    // Set up serialized field for non-schema fields.
    $options[$this->serializedField] = array();

    $dbfields = $this->getDatabaseFields();
    foreach ($options as $key => $value) {
      if (array_search($key, $dbfields) === FALSE) {
        $options[$this->serializedField][$key] = $value;
      }
    }

    $keys = $this->getId() ? array($this->primaryKey) : array();
    drupal_write_record($this->table, $options, $keys);
    return $this;
  }

}

/**
 * An object that holds CourseObjects and tracker functions?
 */
class Course extends CourseHandler {

  private $node;
  private $user;
  protected $courseObjects;
  private $active = NULL;
  private $next;
  private $prev;

  public function __construct($node, $user = NULL) {
    if (is_object($node)) {
      $this->node = $node;
    }
    else {
      $this->node = node_load($node);
    }

    if (is_object($user)) {
      $this->user = $user;
    }
    else {
      $this->user = user_load($user);
    }

    $this->setActive();
  }

  /**
   * The Drupal path to take this course.
   *
   * @return string
   */
  public function getUrl() {
    return "node/{$this->node->nid}/takecourse";
  }

  /**
   * Set the active CourseObject in this Course.
   *
   * @param int $id
   *   A numeric course object ID.
   */
  public function setActive($id = NULL) {
    if (!$id) {
      $id = $_SESSION['course'][$this->node->nid]['taking']['active'];
    }

    foreach ($this->getObjects() as $courseObject) {
      if ($id == $courseObject->getId()) {
        // Active - save old, store next.
        if ($old) {
          $this->prev = $old;
        }

        $storeNext = TRUE;
        $this->active = $courseObject;
      }
      elseif ($storeNext) {
        $this->next = clone $courseObject;
        $storeNext = FALSE;
      }

      $old = clone $courseObject;
    }
  }

  /**
   * Get the active CourseObject.
   *
   * @return CourseObject
   */
  public function getActive() {
    if (!$this->active) {
      $this->setActive();
    }

    return $this->active;
  }

  /**
   * Get the next course object, from the active course object.
   *
   * @return CourseObject
   */
  public function getNext() {
    return $this->next;
  }

  /**
   * Get the previous course object, from the active course object.
   *
   * @return CourseObject
   */
  public function getPrev() {
    return $this->prev;
  }

  /**
   * Track the course (scan requirements, update the progress, completion etc)
   */
  public function track() {
    $sql = "select * from {course_outline} cr
    left join {course_outline_fulfillment} rf on (rf.uid = %d and rf.snid = cr.snid)
    where nid = %d and enabled
    order by weight asc";
    $result = db_query($sql, $this->user->uid, $this->node->nid, $this->user->uid);
    if (!$course_report = course_report_load($this->node, $this->user)) {
      $course_report = new stdClass;
    }
    while ($row = db_fetch_object($result)) {
      if (!$prev) {
        $course_report->section_name = $row->title;
      }

      // Count required objects.
      $required += $row->required;

      // Count completed required objects.
      $required_complete += ($row->required && $row->complete);

      if (!$row->complete && $prev->complete) {
        $course_report->section_name = $row->title;
      }

      $prev = clone $row;
    }

    if ($required_complete >= $required) {
      // Course requirements have been met.
      $course_report->section = 'complete';
      $course_report->section_name = 'Complete';
      $course_report->complete = TRUE;
    }

    $course_report->nid = $this->node->nid;
    $course_report->uid = $this->user->uid;
    course_report_save($course_report);
  }

  /**
   * Get the course objects in this course.
   *
   * @return array
   *   An array of course objects.
   */
  public function getObjects() {
    if (!$this->courseObjects) {
      $this->courseObjects = array();
      $sql = 'SELECT * FROM {course_outline} co
        WHERE nid = %d
        ORDER BY weight ASC';
      $result = db_query($sql, $this->node->nid);

      while ($row = db_fetch_object($result)) {
        $courseObject = course_get_course_object($row, $this->user);
        $courseObject->setCourse($this);
        $this->courseObjects[] = $courseObject;
      }
    }

    return $this->courseObjects;
  }

  public function getNode() {
    return $this->node;
  }

  public function getUser() {
    return $this->user;
  }

}

/**
 * Access handler for CourseObjects.
 *
 * Subtypes must define take(), see(), and view().
 */
abstract class CourseObjectAccess extends CourseHandler {

  private $courseObject;

  function __construct($config = array()) {
    $handlerType = 'course_access';
    parent::__construct($config);
  }

  public function setCourseObject($courseObject) {
    $this->courseObject = $courseObject;
  }

  public function getCourseObject() {
    return $this->courseObject;
  }

  abstract public function take();

  abstract public function see();

  abstract public function view();
}
