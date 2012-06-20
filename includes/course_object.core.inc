<?php

/**
 * Parent abstract base class of all course objects.
 *
 * Represents a course object in the database.
 *
 * Also holds a fulfillment record if a user is given.
 */
class CourseObject extends CourseHandler {

  // Hold course.
  private $course = stdClass;

  // Hold requirement/object from database and the fulfillment object.
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
  function __construct($config = NULL, $account = NULL) {
    $this->serializedField = 'data';
    $this->handlerType = 'course_object';

    // Pass configuration to parent.
    parent::__construct($config);

    if (!$account) {
      global $user;
      $account = $user;
    }

    $this->courseObjectFulfillment = new CourseObjectFulfillment($this, $account);
  }

  /**
   * Access functionality for course objects.
   *
   * Possible values for $op are 'see', 'view', 'take'.
   *
   * "see" means see it in a course outline. For example, a followup survey
   * should not be seen in the course outline. A quiz at the end of the course,
   * should show up, but the user must not be able to take it.
   *
   * "view" means view and interact with the object, but nothing would be
   * recorded. For example, accessing a quiz but not being able to submit
   * responses.
   *
   * Subclasses may override this functionality.
   */
  public function access($op = 'view', $user = NULL) {
    $access = FALSE;

    if (!$user) {
      global $user;
    }

    switch ($op) {
      case 'see':
        // If this is a followup object, this should return FALSE if it's not
        // time to show it.
        return !$this->getOption('hidden');
      case 'take':
        if (!$user->uid) {
          return FALSE;
        }

        if (time() < strtotime($this->getOption('release'))) {
          $this->setAccessMessage('not-open', 'This activity will be available on ' . $this->getOption('release'));
          return FALSE;
        }
        if ($this->getOption('expiration') && time() > strtotime($this->getOption('expiration'))) {
          $this->setAccessMessage('closed', 'This activity closed on ' . $this->getOption('expiration'));
          return FALSE;
        }
      case 'view':
        // Get a copy of the course, so we can run setActive() without changing
        // the global course.
        $course = clone $this->getCourse();
        $course->setActive($this->getId());

        if ($course->key() == 0 && $course->current()->getId() === $this->getId()) {
          // Check if this is the first object.
          $access = TRUE;
        }

        if ($course->getPrev() && $course->getPrev()->getFulfillment()->isComplete()) {
          // If last object was complete, and we are on the current object,
          // grant access.
          $access = TRUE;
        }

        return $access;
    }
  }

  public function isActive() {
    return $this->getCourse()->current()->getId() == $this->getId();
  }

  /**
   * Define configuration elements and their defaults.
   *
   * Extended classes should call parent::optionsDefinition first to get the
   * parent's configuration.
   */
  public function optionsDefinition() {
    $defaults = parent::optionsDefinition();

    $defaults += array(
      'hidden' => 0,
      'instance' => NULL,
      'duration' => NULL,
      'release' => NULL,
      'expiration' => NULL,
    );

    return $defaults;
  }

  /**
   * Default options form for all course objects.
   */
  public function optionsForm(&$form, &$form_state) {
    parent::optionsForm($form, $form_state);

    $config = $this->getOptions();

    $form['header']['#value'] = t("<h2>Settings for %t</h2>", array('%t' => $this->getTitle()));

    $form['uniqid'] = array(
      '#type' => 'hidden',
      '#value' => arg(4),
    );

    $form['nid'] = array(
      '#type' => 'hidden',
      '#value' => arg(1),
    );

    $form['hidden'] = array(
      '#title' => t('Hidden'),
      '#type' => 'checkbox',
      '#description' => t('Hide this course object in the course outline. If this object is required, progress through the course will be blocked until this object is complete.'),
      '#default_value' => $config['hidden'],
    );

    $form['duration'] = array(
      '#title' => t('Duration'),
      '#description' => 'Length of time in minutes a user can remain in this object before it is closed.',
      '#type' => 'textfield',
      '#size' => 2,
      '#default_value' => $config['duration'],
    );

    $form['release'] = array(
      '#title' => t('Release date'),
      '#description' => t('When this object can be accessed. If this object is required, users will not be able to proceed until after this date.'),
      '#type' => 'date_popup',
      '#default_value' => $config['release'],
    );

    $form['expiration'] = array(
      '#title' => t('Expiration date'),
      '#description' => t('When this object will close. If this object is required, users will not be able to proceed to the next activity after this date.'),
      '#type' => 'date_popup',
      '#default_value' => $config['expiration'],
    );

    // Submit
    $form['submit'] = array(
      '#value' => 'Save',
      '#weight' => 999,
      '#type' => 'submit',
    );
  }

  /**
   * Save object configs to cache.
   */
  public function optionsSubmit(&$form, &$form_state) {
    foreach (array_keys($this->getOptions()) as $key) {
      $_SESSION['course'][$this->getCourseNid()]['editing'][$form_state['values']['uniqid']][$key] = $form_state['values'][$key];
    }
  }

  /**
   * Get options, with session options having precedence.
   */
  public function getOptions() {
    $options = parent::getOptions();
    $sessionDefaults = array_filter((array) $_SESSION['course'][$options['nid']]['editing'][$options['snid']], create_function('$a', 'return !is_null($a);'));
    return array_merge($options, (array) $sessionDefaults);
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

    // Run access checks.
    if (!$this->access('take')) {
      return FALSE;
    }

    // Save fulfillment record.
    $courseObject->getFulfillment()->save();

    // If we're not displaying any content but we want to fire take() anyway, to
    // let the course object know we sent the user.
    $out = $this->take();

    $url = $this->getTakeUrl();
    switch ($this->getTakeType()) {
      case 'iframe':
        return course_iframe($url);
      case 'popup':
        return "will popup $url";
      case 'content':
        return $out;
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
   * Return the URL to the course object router.
   */
  public function getUrl() {
    return 'node/' . $this->getCourseNid() . '/course-object/' . $this->getId();
  }

  /**
   * Get the URL to take this course object, if any.
   *
   * Outline handlers or subclasses should use getUrl().
   *
   * @return string
   */
  protected function getTakeUrl() {

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
    return (bool) $this->getOption('required');
  }

  /**
   * Get the user's status in this course object.
   *
   * This is how an object would notify the user why they cannot proceed to the
   * next step from the course outline. For example, if this was a quiz and
   * they failed, this should let them know.
   */
  public function getStatus($type = 'short') {
    /*
      if (!$this->courseObjectFulfillment->isComplete()) {
      if ($type == 'short') {
      return "Failed";
      }
      if ($type == 'long') {
      return "You received an 80% on this quiz but the minimum grade is 90%. You may retake this quiz 5 times.";
      }
      }
     */
  }

  /**
   * Get the course object ID of this course object.
   *
   * @return int
   */
  public function getId() {
    return $this->getOption('snid');
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
    return $this->getOption('instance');
  }

  /**
   * Set this object's instance ID.
   *
   * @param mixed $id The external ID of this course object.
   */
  function setInstanceId($id) {
    $this->setOption('instance', $id);
  }

  /**
   * Get the course node ID this object belongs to.
   */
  function getCourseNid() {
    return intval($this->getOption('nid'));
  }

  function setCourse($course) {
    $this->course = $course;
  }

  function getCourse() {
    return $this->course;
  }

  /**
   * Get the module that provides this course object.
   */
  function getModule() {
    return $this->getOption('requirement_type');
  }

  /**
   * Get the object component for this course object.
   */
  function getComponent() {
    return $this->getOption('requirement_component');
  }

  /**
   * Set the module that provides this course object.
   */
  function setModule($module) {
    $this->setOption('requirement_type', $module);
  }

  /**
   * Set the object component for this course object.
   */
  function setComponent($component) {
    $this->setOption('requirement_component', $component);
  }

  /**
   * Set the course node ID this object belongs to.
   *
   * @param int $nid
   *   The node ID this object should be added to.
   */
  function setCourseNid($nid) {
    $this->setOption('nid', $nid);
  }

  /**
   * Set the internal course object ID.
   *
   * @param int $snid
   *   ID of the course object.
   */
  function setId($snid) {
    $this->setOption('snid', $snid);
  }

  /**
   * Save the course object to the database.
   *
   * @return CourseObject
   */
  public function save() {
    $options = $this->getOptions();
    if (!$options['uuid']) {
      $options['uuid'] = uuid_uuid();
    }

    // Set up serialized field for non-schema fields.
    $options['data'] = array();

    $dbfields = $this->getDatabaseFields();
    foreach ($options as $key => $value) {
      if (array_search($key, $dbfields) === FALSE) {
        $options['data'][$key] = $value;
      }
    }

    $keys = $this->getId() ? array('snid') : array();
    drupal_write_record('course_outline', $options, $keys);
    return $this;
  }

  /**
   * Creates a course object.
   *
   * For example, this would create the new node and return the node ID if this
   * was a CourseObjectNode.
   *
   * Do not confuse this with save(), which saves the course outline record for
   * tracking.
   *
   * Course objects should call setInstanceId() if this is a course object
   * that creates external resources.
   */
  public function create() {}

  /**
   * Deletes a course object's external resources.
   *
   * For example, this would delete the associated node (if this was a
   * CourseObjectNode) and delete all other associated data.
   */
  public function delete() {}

  function getTitle() {
    return $this->getOption('title');
  }

  /**
   * Give the course object a chance to check for fulfillment and set completion
   * on demand.
   *
   * Useful for external objects.
   */
  function checkFulfillment() {

  }

  protected function getDatabaseFields() {
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

}

/**
 * Parent class for course object fulfillment.
 *
 * Represents the fulfillment record in the database.
 *
 */
class CourseObjectFulfillment extends CourseHandler {

  // Fulfillment array.
  private $fulfillment = stdClass;
  private $courseObject;

  public function getId() {
    return $this->fulfillment->sfid;
  }

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
    if (!$this->fulfillment->date_completed) {
      $this->fulfillment->date_completed = time();
    }

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
    if (!$this->fulfillment->uuid) {
      $this->fulfillment->uuid = uuid_uuid();
    }

    if (!$this->fulfillment->date_started) {
      $this->fulfillment->date_started = time();
    }

    $keys = $this->fulfillment->sfid ? array('sfid') : array();
    $ret = drupal_write_record('course_outline_fulfillment', $this->fulfillment, $keys);

    // Create a Course to track the user's progress.
    $course = new Course($this->getCourseObject()->getCourseNid(), $this->fulfillment->uid);
    $course->track();

    return $ret;
  }

  public function delete() {
    $sql = "delete from {course_outline_fulfillment} where sfid = %d";
    db_query($sql, $this->getId());
  }

  function setData($param, $data) {
    $this->fulfillment->info[$param] = $data;
  }

}

/**
 * A course object that uses a node as a base.
 */
class CourseObjectNode extends CourseObject {

  protected $node;

  public function __construct($requirement, $user = NULL) {
    // Pass configuration to parent.
    parent::__construct($requirement, $user);
    $this->node = node_load($this->getInstanceId());
  }

  public function checkPrivacySupport() {
    return module_exists('content_access') && module_exists('acl');
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

  public function optionsDefinition() {
    $defaults = parent::optionsDefinition();

    $defaults['private'] = 0;

    return $defaults;
  }

  public function optionsForm(&$form, &$form_state) {
    parent::optionsForm($form, $form_state);

    $instance = $this->getOption('instance');

    $config = $this->getOptions();

    $form['instance'] = array(
      '#title' => 'Existing node',
      '#autocomplete_path' => 'ctools/autocomplete/node',
      '#type' => 'textfield',
    );

    $form['private'] = array(
      '#title' => t('Private'),
      '#description' => $this->checkPrivacySupport() ? t('This content will not be available to users who are not enrolled in this course.') : t('You must enable content_access and acl in order to restrict course content to users who are enrolled in this course.'),
      '#type' => 'checkbox',
      '#default_value' => $config['private'],
      '#disabled' => !($this->checkPrivacySupport()),
    );

    $nid = $this->getInstanceId();
    if ($nid) {
      $node = node_load($nid);
      $link = l(t("'%title' [node id %nid]", array('%title' => $node->title, '%nid' => $node->nid)), "node/$node->nid", array('attributes' => array('target' => '_blank', 'title' => t('Open in new window')), 'html' => TRUE));
      $form['instance']['#description'] = t('Currently set to !link', array('!link' => $link));
    }
  }

  public function optionsSubmit(&$form, &$form_state) {
    $nid = $form_state['values']['instance'];

    if (!is_numeric($nid)) {
      preg_match('/^(?:\s*|(.*) )?\[\s*nid\s*:\s*(\d+)\s*\]$/', $nid, $matches);
      $nid = $matches[2];
    }

    if ($nid) {
      $form_state['values']['instance'] = $nid;
    }
    else {
      // Unset it, or we'll erase the relationship (since the textfield is
      // actually blank).
      unset($form_state['values']['instance']);
    }

    parent::optionsSubmit($form, $form_state);
  }

  public function getWarnings() {
    $warnings = parent::getWarnings();
    if ($this->getOption('private')) {
      $settings = variable_get('content_access_settings', array());

      if (!$settings['per_node'][$this->getComponent()]) {
        $warnings[] = t('%t is set to Private, but the content type %c does not have per-content restriction <strong>and is visible to all users</strong>. Please visit !l to set up content access settings.', array(
          '%t' => $this->getTitle(),
          '%c' => $this->getComponent(),
          '!l' => l('Access control', "admin/content/node-type/{$this->getComponent()}/access"),
          ));
      }
    }
    return $warnings;
  }

  /**
   * Grant access to course content before going to it.
   */
  function takeCourseObject() {
    if ($this->checkPrivacySupport()) {
      global $user;
      if ($this->getOption('private')) {
        module_load_include('inc', 'content_access', 'content_access.admin');
        $acl_id = content_access_get_acl_id($this->node, 'view');
        acl_add_user($acl_id, $user->uid);
        acl_node_add_acl($this->node->nid, $acl_id, 1, 0, 0, content_access_get_settings('priority', $this->node->type));
        node_save($this->node);
      }
    }
    return parent::takeCourseObject();
  }

}
