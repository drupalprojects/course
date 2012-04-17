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
  private $requirement = stdClass;
  private $courseObjectFulfillment = stdClass;

  /**
   * Construct a course object from a database record.
   *
   * Use course_get_course_object to load an object when parameters are not
   * already known.
   *
   * @param object $requirement
   *   An object with requirement_component, requirement_type, and instance
   * @param object $user
   */
  function __construct($requirement = NULL, $account = NULL) {
    if (!$user) {
      global $user;
      $account = $user;
    }

    $this->requirement = $requirement;

    $config = (array) unserialize($this->requirement->config);

    foreach ($config as $key => $value) {
      if (!in_array($key, $this->getDatabaseFields())) {
        $this->requirement->$key = $value;
      }
    }

    unset($this->requirement->config);

    $this->courseObjectFulfillment = new CourseObjectFulfillment($this, $account);
  }

  /**
   * Define configuration elements and their defaults.
   *
   * Extended classes should call parent::optionsDefinition first to get the
   * parent's configuration.
   */
  public function optionsDefinition() {
    return array();
  }

  /**
   * Default options form for all course objects.
   */
  public function optionsForm(&$form, &$form_state) {
    $form = array();

    $form['header']['#value'] = t("<h2>Settings for %t</h2>", array('%t' => $this->getTitle()));

    $form['uniqid'] = array(
      '#type' => 'hidden',
      '#value' => arg(4),
    );

    $form['nid'] = array(
      '#type' => 'hidden',
      '#value' => arg(1),
    );

    /*
      $form['duration'] = array(
      '#title' => 'Length of time in minutes a user can remain in this object before it is closed.',
      '#type' => 'textfield',
      '#size' => 2,
      );

      $form['release'] = array(
      '#title' => 'When this object can be accessed.',
      '#type' => 'date',
      );

      $form['expiration'] = array(
      '#title' => 'When this object will close.',
      '#type' => 'date',
      );
     */

    // Submit
    $form['submit'] = array(
      '#value' => 'Save',
      '#weight' => 999,
      '#type' => 'submit',
    );

    return $form;
  }

  /**
   * Validate?
   */
  public function optionsValidate(&$form, &$form_state) {

  }

  /**
   * Save object configs to cache.
   */
  public function optionsSubmit(&$form, &$form_state) {
    foreach (array_keys($this->optionsDefinition()) as $key) {
      $_SESSION['course'][$this->getCourseNid()]['editing'][$form_state['values']['uniqid']][$key] = $form_state['values'][$key];
    }
  }

  /**
   * Take a course object.
   *
   * - Set the session of this course object being taken. This allows for
   *   non-node objects to be tracked.
   * - Delegate the course object take functionality
   *
   * @return mixed
   *   HTML content or a redirect.
   */
  public function takeCourseObject() {
    $_SESSION['course'][$this->getCourseNid()]['taking']['active'] = $this->getId();

    $url = $this->getTakeUrl();
    switch ($this->getTakeType()) {
      case 'iframe':
        return course_iframe($url);
      case 'popup':
        return "will popup $url";
      case 'content':
        return $this->take();
      case 'redirect':
      default:
        // This URL should have already been url()'d (it might be external).
        session_write_close();
        header("Location: $url");
        exit;
    }
  }

  /**
   * How should this course object be executed?
   *
   * - iframe: display an iframe with getTakeUrl() in it
   * - popup: launch getTakeUrl() in a popup
   * - modal: launch getTakeUrl() in a modal
   * - content: print the value from take() (or do whatever the module wants to
   *   do)
   */
  public function getTakeType() {
    return 'content';
  }

  /**
   *
   */
  public function take() {
    return 'This should be overridden by the module to return course content.';
  }

  /**
   * Get the URL to take this course object, if any.
   *
   * @return string
   */
  public function getTakeUrl() {

  }

  /**
   * Get the URL to edit this course object, if any.
   *
   * @return string
   */
  public function getEditUrl() {

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
   * Set this object's instance ID.
   *
   * @param mixed $id The external ID of this course object.
   */
  function setInstanceId($id) {
    $this->requirement->instance = $id;
  }

  /**
   * Get the course node ID this object belongs to.
   */
  function getCourseNid() {
    return intval($this->requirement->nid);
  }

  /**
   * Get the object component for this course object.
   */
  function getComponent() {
    return $this->requirement->requirement_component;
  }

  /**
   * Set the course node ID this object belongs to.
   *
   * @param int $nid The node ID this object should be added to.
   */
  function setCourseNid($nid) {
    $this->requirement->nid = $nid;
  }

  function setId($snid) {
    $this->requirement->snid = $snid;
  }

  /**
   * Save the course object to the database.
   */
  public function save() {
    $keys = $this->requirement->snid ? array('snid') : array();
    return drupal_write_record('course_outline', $this->requirement, $keys);
  }

  /**
   * Creates a course object.
   *
   * For example, this would create the new node and return the node ID if this
   * was a CourseObjectNode.
   *
   * Do not confuse this with save(), which saves the course outline record for
   * tracking.
   */
  public function create() {
    // Stub. This should set $this->setInstanceId().
  }

  /**
   * Deletes a course object.
   *
   * For example, this would delete the associated node (if this was a
   * CourseObjectNode) and delete all other associated data.
   */
  public function delete() {
    // Stub. Clean up everything appropriate to this object.
  }

  /**
   * Get an object's configuration.
   */
  public function getOptions() {
    $defaults = $this->optionsDefinition();
    $sessionDefaults = $_SESSION['course'][$this->getCourseNid()]['editing'][$this->getId()];
    return array_merge($defaults, (array) $this->requirement, (array) $sessionDefaults);
  }

  /**
   * Get an option.
   */
  public function getOption($key) {
    $config = $this->getOptions();
    return $config[$key];
  }

  public function setOption($option, $value) {
    if (in_array($option, $this->getDatabaseFields())) {
      $this->requirement->$option = $value;
    }
    else {
      $this->requirement->config[$option] = $value;
    }
    return $this;
  }

  private function getDatabaseFields() {
    return array(
      'snid',
      'nid',
      'requirement_type',
      'title',
      'requirement_component',
      'enabled',
      'instance',
      'required',
      'weight',
      'hidden',
    );
  }

  function getTitle() {
    return $this->requirement->title;
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
    $this->fulfillment = db_fetch_object(db_query($sql, $this->courseObject->getId(), $user->uid));

    // Set requirement ID.
    $this->fulfillment->snid = $this->courseObject->getId();
    $this->fulfillment->uid = $user->uid;
  }

  /**
   * Is this fulfillment complete?
   *
   * @return bool
   */
  function isComplete() {
    return (bool) $this->fulfillment->complete;
  }

  /**
   * Set this fulfillment complete.
   *
   * @param bool $complete
   *   Set to 0 to un-complete, 1 or omit to complete.
   */
  function setComplete($complete = 1) {
    $this->fulfillment->complete = $complete;
    return $this;
  }

  /**
   * Set this fulfillment's grade.
   *
   * @param float $grade
   */
  function setGrade($grade) {
    $this->fulfillment->grade_result = $grade;
    return $this;
  }

  /**
   * Get this fulfillment's grade.
   *
   * @return float
   *   A float value of the user's grade for this fulfillment.
   */
  function getGrade() {
    return $this->fulfillment->grade_result;
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
    $keys = $this->fulfillment->sfid ? array('sfid') : array();
    $ret = drupal_write_record('course_outline_fulfillment', $this->fulfillment, $keys);

    // Create a Course to track the user's progress.
    $course = new Course($this->getCourseObject()->getCourseNid(), $this->fulfillment->uid);
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

  /**
   * Simple node course object behavior is to just redirect to the node.
   */
  public function getTakeType() {
    return 'redirect';
  }

  public function getTakeUrl() {
    return url("node/{$this->node->nid}");
  }

  public function getEditUrl() {
    return url("node/{$this->node->nid}/edit");
  }

  public function create() {
    $content = new stdClass;
    $content->type = $this->getComponent();
    $content->title = $this->getTitle();
    $content->uid = $this->node->uid;
    node_save($content);
    $this->setInstanceId($content->nid);
  }

  /**
   * Destroy the node instance.
   */
  public function delete() {
    node_delete($this->getInstanceId());
  }

  public function optionsForm(&$form, &$form_state) {
    $form = parent::optionsForm($form, $form_state);

    $form['instance'] = array(
      '#title' => 'Existing node',
      '#autocomplete_path' => 'ctools/autocomplete/node',
      '#type' => 'textfield',
    );

    $nid = $this->getInstanceId();
    if ($nid) {
      $node = node_load($nid);
      $link = l(t("'%title' [node id %nid]", array('%title' => $node->title, '%nid' => $node->nid)), "node/$node->nid", array('attributes' => array('target' => '_blank', 'title' => t('Open in new window')), 'html' => TRUE));
      $form['instance']['#description'] = t('Currently set to !link', array('!link' => $link));
    }

    return $form;
  }

  public function optionsSubmit(&$form, &$form_state) {
    $value = $form_state['values']['instance'];
    preg_match('/^(?:\s*|(.*) )?\[\s*nid\s*:\s*(\d+)\s*\]$/', $value, $matches);
    $nid = $matches[2];
    if ($nid) {
      $this->setOption('instance', $nid);
    }
    parent::optionsSubmit($form, $form_state);
  }

}
