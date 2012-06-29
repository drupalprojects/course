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
  private $course = NULL;
  // Hold requirement/object from database and the fulfillment object.
  private $courseObjectFulfillment = NULL;

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
  function __construct($config = array(), $account = NULL) {
    $config = (array) $config;

    $this->serializedField = 'data';
    $this->handlerType = 'course_object';
    $this->primaryKey = 'snid';
    $this->table = 'course_outline';


    // Pass configuration to parent.
    parent::__construct($config);

    if (!$account) {
      global $user;
      $account = $user;
    }

    if ($config['nid']) {
      // Object is being constructed as part of a course.
      $courseNode = node_load($config['nid']);
      $this->course = course_get_course($courseNode);
    }

    $this->courseObjectFulfillment = new CourseObjectFulfillment($this, $account);
  }

  /**
   * Access functionality for course objects.
   *
   * Possible values for $op are 'see', 'view', 'take'.
   *
   * "see" means see it in a course outline. For example, a conditional survey
   * should not be seen in the course outline. A quiz at the end of the course,
   * should show up, but the user must not be able to take it.
   *
   * "view" means view and interact with the object, but nothing would be
   * recorded. For example, accessing a quiz but not being able to submit
   * responses.
   *
   * "take" means interact with the object in a way that records data.
   *
   * Subclasses may override this functionality.
   */
  public function access($op = 'view', $user = NULL) {
    ctools_include('plugins');
    $access = FALSE;

    if (!$user) {
      global $user;
    }

    // Stock access: check for completion of previous object.
    switch ($op) {
      case 'see':
        $access = !$this->getOption('hidden');
        break;
      case 'take':
        if (!$user->uid) {
          return FALSE;
        }
      case 'view':
        // Get a copy of the course, so we can run setActive() without changing
        // the global course.
        $course = clone $this->getCourse();
        $course->setActive($this->getId());


        if (reset($course->getObjects())->getId() == $this->getId()) {
          // Check if this is the first object.
          $access = TRUE;
        }

        if ($course->getPrev() && $course->getPrev()->getFulfillment()->isComplete()) {
          // If last object was complete, and we are on the current object,
          // grant access.
          $access = TRUE;
        }
    }

    // Plugin access.

    foreach (ctools_get_plugins('course', 'access') as $key => $plugin) {
      $class = ctools_plugin_get_class($plugin, 'handler');
      $accessPluginDefaults = (array) $this->config['plugins']['access'][$key];
      $accessPlugin = new $class();
      if ($accessPluginDefaults) {
        $accessPlugin->setOptions($accessPluginDefaults);
      }
      $accessPlugin->setCourseObject($this);

      // Run access check.
      $ret = $accessPlugin->$op();

      if ($ret === FALSE) {
        // If previous access was granted, revoke it.
        $access = $ret;
      }
    }

    return $access;
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
      'uniqid' => NULL,
      'nid' => NULL,
      'title' => NULL,
      'enabled' => 1,
      'hidden' => 0,
      'required' => 0,
      'delete' => 0,
      'delete_instance' => 0,
    );

    return $defaults;
  }

  /**
   * Default options form for all course objects.
   */
  public function optionsForm(&$form, &$form_state) {
    ctools_include('dependent');
    ctools_include('plugins');
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

    $form['title'] = array(
      '#title' => 'Title',
      '#type' => 'textfield',
      '#size' => 16,
      '#default_value' => check_plain($config['title']),
    );

    $form['enabled'] = array(
      '#title' => 'Enabled',
      '#type' => 'checkbox',
      '#default_value' => $config['enabled'],
    );

    $form['hidden'] = array(
      '#title' => t('Visible in outline'),
      '#type' => 'checkbox',
      //'#description' => t('Hide this course object in the course outline. If this object is required, progress through the course will be blocked until this object is complete.'),
      '#default_value' => !$config['hidden'],
    );

    $form['required'] = array(
      '#title' => t('Completion required'),
      '#type' => 'checkbox',
      '#default_value' => $config['required'],
    );

    // Only allow deletion of existing objects.
    // @todo do we want to allow `delete` when there is a cached object, but
    // only allow `delete_instance` when there is a saved snid?
    if ($config['instance']) {
      $form['delete'] = array(
        '#title' => t('Delete'),
        '#type' => 'checkbox',
      );

      $form['delete_instance'] = array(
        '#title' => t('Also delete related object instance(s)'),
        '#type' => 'checkbox',
        '#process' => array('ctools_dependent_process'),
        '#dependency' => array('edit-delete' => array(1)),
      );
    }

    // Bring in access plugin configuration.
    $form['plugins']['#tree'] = TRUE;
    $form['plugins']['#weight'] = 998;
    $form['plugins']['access']['#title'] = 'Access plugins';
    $form['plugins']['access']['#type'] = 'fieldset';
    foreach (ctools_get_plugins('course', 'access') as $key => $plugin) {
      $form['plugins']['access']['#tree'] = TRUE;
      $form['plugins']['access'][$key] = array(
        '#title' => $plugin['title'],
        '#type' => 'fieldset',
        '#tree' => TRUE,
        '#collapsible' => TRUE,
      );

      // Initialize access class.
      $class = ctools_plugin_get_class($plugin, 'handler');
      $courseAccessDefaults = $config['plugins']['access'][$key];
      $courseAccess = new $class();
      if ($courseAccessDefaults) {
        $courseAccess->setOptions($courseAccessDefaults);
      }
      $courseAccess->setCourseObject($this);

      // Add access plugin form to our form.
      $form['plugins']['access'][$key] += $courseAccess->optionsForm();
    }

    // Update settings
    $form['update'] = array(
      '#value' => t('Update'),
      '#weight' => 999,
      '#type' => 'submit',
    );
  }

  public function optionsValidate(&$form, &$form_state) {
    // Pass validation to plugins.
    ctools_include('plugins');
    foreach (ctools_get_plugins('course', 'access') as $key => $plugin) {
      $values = & $form_state['values']['plugins']['access'][$key];
      $class = ctools_plugin_get_class($plugin, 'handler');
      $instance = new $class($values);
      $instance->optionsValidate($values);
    }
  }

  /**
   * Save object configs to cache.
   */
  public function optionsSubmit(&$form, &$form_state) {
    ctools_include('plugins');

    $uniqid = $this->getId();
    $nid = $this->getCourseNid();

    // Flip 'visible' so it behaves like 'hidden'.
    $form_state['values']['hidden'] = ($form_state['values']['hidden'] != 1);

    // Object-specific settings
    foreach (array_keys($this->getOptions()) as $key) {
      if (!is_null($form_state['values'][$key])) {
        $_SESSION['course'][$nid]['editing'][$uniqid][$key] = $form_state['values'][$key];
      }
    }

    // Save plugin info.
    foreach (ctools_get_plugins('course', 'access') as $key => $plugin) {
      $_SESSION['course'][$nid]['editing'][$uniqid]['plugins']['access'][$key] = $form_state['values']['plugins']['access'][$key];
    }
  }

  /**
   * Get options, with session options having precedence.
   */
  public function getOptions() {
    $options = parent::getOptions();
    $sessionDefaults = array_filter((array) $_SESSION['course'][$options['nid']]['editing'][$options['snid']], array($this, 'optionFilter'));
    return array_merge($options, (array) $sessionDefaults);
  }

  private function optionFilter($a) {
    return !is_null($a);
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
    $_SESSION['course']['active'] = $this->getCourseNid();
    $_SESSION['course'][$this->getCourseNid()]['taking']['active'] = $this->getId();

    // Run access checks.
    if ($this->access('take')) {
      // Grant access to external course object.
      $this->grant();

      // Record start date.
      if (!$this->getFulfillment()->getOption('date_started')) {
        $this->getFulfillment()->setOption('date_started', time());
      }
    }
    else {
      // User can't access this object, revoke access.
      $this->revoke();
      return FALSE;
    }

    // Save fulfillment record.
    $this->getFulfillment()->save();

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
   * Get the course node ID this CourseObject belongs to.
   *
   * @return int
   */
  function getCourseNid() {
    return intval($this->getOption('nid'));
  }

  /**
   * Set the Course for this CourseObject.
   *
   * @param Course $course
   *
   * @return Course
   */
  function setCourse($course) {
    $this->course = $course;
    $this->setOption('nid', $course->getNode()->nid);
    return $this;
  }

  /**
   * Get the Course that contains this CourseObject.
   *
   * @return Course
   */
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
    $courseNode = node_load($nid);
    $this->setCourse(course_get_course($courseNode));
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
  public function create() {
    //$this->setInstanceId($id);
  }

  /**
   * Deletes a course object's external resources.
   *
   * For example, this would delete the associated node (if this was a
   * CourseObjectNode) and delete all other associated data.
   */
  public function delete() {
    //something_delete($this->getInstanceId());
  }

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

  /**
   * Grant access to the external course object.
   *
   * For example, adding a user to an access control list.
   *
   * @see CourseObjectNode::grant()
   */
  function grant() {

  }

  /**
   * Revoke access to the external course object.
   *
   * For example, removing a user to an access control list.
   *
   * @todo This isn't called anywhere useful, yet.
   * @see CourseObjectNode::revoke()
   */
  function revoke() {

  }

  /**
   * Let the course object provide its own reports.
   *
   * @return array
   *   An array indexed by report key, containing 'title' which is the menu link
   *   in the course object reports.
   *
   * @see CourseObjectQuiz::getReports()
   */
  function getReports() {
    return array();
  }

  /**
   * Let the course object provide its own reports.
   *
   * @return array
   *   An array containing:
   *     - title: The title of this report as show on the page
   *     - content: Content to be displayed.
   *     - url: URL to be loaded in an iframe.
   *   Reports should return either 'content' or 'url'.
   *
   * @see CourseObjectQuiz::getReports()
   */
  function getReport($key) {
    return array();
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
    return $this->getOption('sfid');
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
    // Set storage.
    $this->handlerType = 'course_object_fulfillment';
    $this->table = 'course_outline_fulfillment';
    $this->primaryKey = 'sfid';
    $this->serializedField = 'info';

    $this->courseObject = $courseObject;
    $sql = "select * from {course_outline_fulfillment} where snid = %d and uid = %d";
    $fulfillment = db_fetch_array(db_query($sql, $this->courseObject->getId(), $user->uid));

    $this->setOptions($fulfillment);

    // Set requirement ID.
    $this->setOption('snid', $this->courseObject->getId());
    $this->setOption('uid', $user->uid);
  }

  /**
   * Is this fulfillment complete?
   *
   * @return bool
   */
  function isComplete() {
    return (bool) $this->getOption('complete');
  }

  /**
   * Set this fulfillment complete.
   *
   * @param bool $complete
   *   Set to 0 to un-complete, 1 or omit to complete.
   */
  function setComplete($complete = 1) {
    if (!$this->getOption('date_completed')) {
      $this->setOption('date_completed', time());
    }

    $this->setOption('complete', $complete);
    return $this;
  }

  /**
   * Set this fulfillment's grade.
   *
   * @param float $grade
   */
  function setGrade($grade) {
    $this->setOption('grade', $grade);
    return $this;
  }

  /**
   * Get this fulfillment's grade.
   *
   * @return float
   *   A float value of the user's grade for this fulfillment.
   */
  function getGrade() {
    return $this->getOption('grade_result');
  }

  /**
   * Get this fulfillment's course object.
   */
  function getCourseObject() {
    return $this->courseObject;
  }

  public function delete() {
    $sql = "delete from {course_outline_fulfillment} where sfid = %d";
    db_query($sql, $this->getId());
  }

  /**
   * Track course after saving fulfillment.
   */
  public function save() {
    parent::save();
    $this->getCourseObject()->getCourse()->track();
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
        $warnings[] = t('%t is set to Private, but the content type %c does not have access control lists enabled. Users will not be able to acces this content. Please visit !l to set up content access settings.', array(
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
  function grant() {
    if ($this->checkPrivacySupport()) {
      if ($this->getOption('private')) {
        $uid = $this->getCourse()->getUser()->uid;
        module_load_include('inc', 'content_access', 'content_access.admin');
        $acl_id = content_access_get_acl_id($this->node, 'view');
        acl_add_user($acl_id, $uid);
        acl_node_add_acl($this->node->nid, $acl_id, 1, 0, 0, content_access_get_settings('priority', $this->node->type));
        node_save($this->node);
      }
    }
  }

  /**
   * Duration expired (or something) - CourseObject is telling us so.
   */
  function revoke() {
    if ($this->checkPrivacySupport()) {
      if ($this->getOption('private')) {
        $uid = $this->getCourse()->getUser()->uid;
        module_load_include('inc', 'content_access', 'content_access.admin');
        $acl_id = content_access_get_acl_id($this->node, 'view');
        acl_remove_user($acl_id, $uid);
        node_save($this->node);
      }
    }
  }

  /**
   * On object write, set privacy on this node.
   */
  function save() {
    parent::save();
    if ($this->checkPrivacySupport() && $this->getOption('private')) {
      // Remove "view" permissions to everyone on this node.
      $settings = content_access_get_per_node_settings($this->node);
      $settings['view'] = array();
      content_access_save_per_node_settings($this->node, $settings);
      // Resave node to update access.
      node_save($this->node);
    }
  }

}
