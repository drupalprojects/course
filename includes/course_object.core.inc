<?php

/**
 * Parent abstract base class of all course objects.
 *
 * Represents a course object in the database.
 *
 * Also holds a fulfillment record if a user is given.
 */
abstract class CourseObject extends CourseHandler {

  // Hold course.
  private $course = NULL;
  // Hold course object from database and the fulfillment object.
  private $courseObjectFulfillment = NULL;
  // Hold a user.
  protected $user = NULL;

  /**
   * Construct a course object from a database record.
   *
   * Use course_get_course_object to load an object when parameters are not
   * already known.
   *
   * @param array $config
   *   An object with object_type, module, and instance
   * @param object $account
   *   (optional) A user account object.
   * @param Course $course
   *   (optional) An instantiated Course object.
   */
  function __construct($config = array(), $account = NULL, Course $course = NULL) {
    $config = (array) $config;

    $this->serializedField = 'data';
    $this->entity_type = 'course_object';
    $this->primaryKey = 'coid';
    $this->table = 'course_outline';


    // Pass configuration to parent.
    parent::__construct($config);

    if (!$account) {
      global $user;
      $account = $user;
    }
    $this->user = $account;

    if ($course) {
      $this->course = $course;
    }

    $this->courseObjectFulfillment = new CourseObjectFulfillment($this, $account);
  }

  /**
   * Override navigation links.
   *
   * @return array
   *   An array of navigation links. Keyed values will override matching items
   *   in Course::getNavigation().
   */
  public function overrideNavigation() {
    return array();
  }

  /**
   * Specify whether fulfillment uses asynchronous polling.
   *
   * @return bool
   *   Whether this object uses polling. Defaults to FALSE.
   */
  public function hasPolling() {
    return FALSE;
  }

  /**
   * Overrides a course outline list item.
   *
   * @param array $item
   *   A course outline list item. The structure mirrors an array element from
   *   the $items param from theme_item_list().
   */
  public function overrideOutlineListItem(&$item) {

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
  public function access($op = 'view', $account = NULL) {
    ctools_include('plugins');
    $access = FALSE;

    if (!$account) {
      global $user;
      $account = $user;
    }

    // Stock access: check for completion of previous object.
    switch ($op) {
      case 'see':
        $access = !$this->getOption('hidden') && $this->getOption('enabled');
        break;
      case 'take':
        if (!$account->uid) {
          return FALSE;
        }
      case 'view':
        // Get a copy of the course, so we can run setActive() without changing
        // the global course.
        $course = clone $this->getCourse();
        $course->setActive($this->getId());
        $courseObjects = $course->getObjects();

        // Deny object access to non-enrolled users.
        if (!course_enrollment_check($this->getCourseNid(), $account->uid)) {
          return FALSE;
        }
        else if ($courseObjects[0]->getId() == $this->getId()) {
          // User is enrolled. Grant access if first object.
          $access = TRUE;
        }

        if ($course->getPrev() && (!$course->getPrev()->isRequired() || ($course->getPrev()->isRequired() && $course->getPrev()->isSkippable()))) {
          // Previous step was not required.
          $access = TRUE;

          // But we need to see if at least one required step was completed (or the start of the course).
          $objects = array_reverse($course->getObjects());
          $check = FALSE;
          foreach ($objects as $object) {
            if ($check) {
              if ($object->isRequired() && !$object->isSkippable()) {
                // Object is required.
                if (!$object->getFulfillment()->isComplete()) {
                  // Found a required object that was not complete.
                  $access = FALSE;
                  break;
                }
                else {
                  // The last required object was completed.
                  $access = TRUE;
                  break;
                }
              }
            }
            if ($object->getId() == $this->getId()) {
              // We found the object we are trying to check access on.
              // Now we want to go backwards.
              $check = 1;
            }
          }
        }

        if ($course->getPrev() && $course->getPrev()->getFulfillment()->isComplete()) {
          // If last object was complete, and we are on the current object,
          // grant access.
          $access = TRUE;
        }
    }

    // Plugin access.

    foreach (ctools_get_plugins('course', 'course_object_access') as $key => $plugin) {
      $class = ctools_plugin_get_class($plugin, 'handler');
      $accessPluginDefaults = array();
      if (isset($this->config['plugins']['access'][$key])) {
        $accessPluginDefaults = (array) $this->config['plugins']['access'][$key];
      }
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
      'required' => 1,
      'skippable' => 0,
      'delete' => 0,
      'delete_instance' => 0,
      'grade_include' => 0,
      'instance' => NULL,
      'data' => '',
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

    $form['header']['#markup'] = t("<h2>Settings for %t</h2>", array('%t' => $this->getTitle()));

    $form['uniqid'] = array(
      '#type' => 'hidden',
      '#value' => arg(4),
    );

    $form['nid'] = array(
      '#type' => 'hidden',
      '#value' => arg(1),
    );

    $form['course_tabs']['#type'] = 'vertical_tabs';

    $form['title'] = array(
      '#title' => t('Title & description'),
      '#type' => 'fieldset',
      '#group' => 'course_tabs',
      '#weight' => 0,
    );


    $form['outline'] = array(
      '#type' => 'fieldset',
      '#title' => t('Settings'),
      '#group' => 'course_tabs',
      '#weight' => 1,
    );

    $form['plugins']['access'] = array(
      '#type' => 'fieldset',
      '#title' => 'Access',
      '#group' => 'course_tabs',
      '#weight' => 4,
    );

    $form['delete'] = array(
      '#type' => 'fieldset',
      '#title' => 'Delete',
      '#group' => 'course_tabs',
      '#weight' => 5,
    );

    $form['title']['title'] = array(
      '#title' => t('Title'),
      '#type' => 'textfield',
      '#description' => t('The title of this course object as it will appear to users in the course outline.'),
      '#size' => 100,
      '#default_value' => check_plain($config['title']),
      '#group' => 'description',
    );

    $form['outline']['enabled'] = array(
      '#title' => t('Enabled'),
      '#type' => 'checkbox',
      '#description' => t('Enabled course objects will become part of the course. Uncheck this box if you are not using this course object.'),
      '#default_value' => $config['enabled'],
    );

    $form['outline']['hidden'] = array(
      '#title' => t('Visible in outline'),
      '#type' => 'checkbox',
      '#description' => t('Objects that are not visible will not be seen by the learner. Uncheck this box for course objects that you do not want the learner to see.'),
      '#default_value' => !$config['hidden'],
      '#group' => 'course',
    );

    $form['outline']['required'] = array(
      '#title' => t('Completion required'),
      '#type' => 'checkbox',
      '#description' => t('Users must complete required objects. Uncheck this box if this is an optional course object.'),
      '#default_value' => $config['required'],
    );

    $form['outline']['skippable'] = array(
      '#title' => t('Skippable'),
      '#type' => 'checkbox',
      '#default_value' => $config['skippable'],
      '#dependency' => array('edit-required' => array(1)),
      '#description' => t('Users can proceed past this object but it will still be required for course completion.'),
    );

    $form['delete']['delete'] = array(
      '#title' => t('Delete'),
      '#type' => 'checkbox',
      '#description' => t('Deleting course content is irreversible.'),
      '#default_value' => $config['delete'],
      '#group' => 'delete',
    );

    // Only allow deletion of existing instances.
    if (!empty($config['instance'])) {
      $form['delete']['delete_instance'] = array(
        '#title' => t('Also delete the referenced content.'),
        '#type' => 'checkbox',
        '#default_value' => $config['delete_instance'],
        '#dependency' => array('edit-delete--2' => array(1)),
        '#group' => 'delete',
      );

      // Check for multiple instances.
      $sql = "SELECT count(coid) FROM {course_outline} WHERE module = '%s' AND object_type = '%s' AND instance = '%s'";
      if (db_query("SELECT count(coid) FROM {course_outline} WHERE module = :module AND object_type = :object_type AND instance = :instance", array(':module' => $config['module'], ':object_type' => $config['object_type'], ':instance' => $config['instance']))->fetchField() > 1) {
        $form['delete_instance']['#description'] = t('<span class="error"><strong>WARNING</strong></span>: multiple course objects link to this instance. Deleting the instance might break the other course objects that use it.');
      }
    }

    if ($this->isGraded()) {
      $form['grading'] = array(
        '#title' => t('Grading'),
        '#type' => 'fieldset',
        '#description' => t('Settings for graded objects.'),
        '#group' => 'course_tabs',
      );

      $form['grading']['grade_include'] = array(
        '#title' => t('Include in final course grade'),
        '#description' => t('Include this grade result for calculation of the final course grade.<br/>Currently, only the last grade result per Course will be used.'),
        '#default_value' => $config['grade_include'],
        '#type' => 'checkbox',
      );
    }

    // Bring in access plugin configuration.
    $form['plugins']['#tree'] = TRUE;
    $form['plugins']['access']['#title'] = t('Access');
    $form['plugins']['access']['#description'] = t('By default, all required objects appearing before this object in the course outline must be completed before the user may access this object. Conditional access allows for additional conditions to be applied.');
    $form['plugins']['access']['#type'] = 'fieldset';
    foreach (ctools_get_plugins('course', 'course_object_access') as $key => $plugin) {
      $form['plugins']['access']['#tree'] = TRUE;
      $form['plugins']['access'][$key] = array(
        '#title' => $plugin['title'],
        '#type' => 'fieldset',
        '#tree' => TRUE,
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );

      // Initialize access class.
      $class = ctools_plugin_get_class($plugin, 'handler');
      $courseAccess = new $class();
      if (!empty($config['plugins']['access'][$key])) {
        $courseAccess->setOptions($config['plugins']['access'][$key]);
      }
      $courseAccess->setCourseObject($this);

      // Add access plugin form to our form.
      $access_form = $access_form_state = array();
      $form['plugins']['access'][$key] += $courseAccess->optionsForm($access_form, $access_form_state);
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
    foreach (ctools_get_plugins('course', 'course_object_access') as $key => $plugin) {
      $values = & $form_state['values']['plugins']['access'][$key];
      $class = ctools_plugin_get_class($plugin, 'handler');
      $instance = new $class($values);
      $instance->optionsValidate($form['plugins']['access'][$key], $form_state['values']['plugins']['access'][$key]);
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
      if (isset($form_state['values'][$key]) && !is_null($form_state['values'][$key])) {
        $_SESSION['course'][$nid]['editing'][$uniqid][$key] = $form_state['values'][$key];
      }
    }

    // Save plugin info.
    foreach (ctools_get_plugins('course', 'course_object_access') as $key => $plugin) {
      $_SESSION['course'][$nid]['editing'][$uniqid]['plugins']['access'][$key] = $form_state['values']['plugins']['access'][$key];
    }

    // Update the options.
    $this->setOptions($_SESSION['course'][$nid]['editing'][$uniqid]);
  }

  /**
   * Get core options summary.
   *
   * @return array
   *   An associative array of summary keys and values.
   */
  public function getOptionsSummary() {
    $summary = parent::getOptionsSummary();

    // Get options.
    $options = $this->getOptions();

    // Add course object core options to summary individually, because there are
    // options we don't want to display, and others that require special logic.
    $uniqid = $options['uniqid'];

    // Enabled.
    if ($options['enabled']) {
      $summary['enabled'] = t('Enabled');
    }
    else {
      $summary['enabled'] = '<span class="warning">' . t('Not enabled') . '</span>';
    }

    // Hidden.
    if (!$options['hidden']) {
      $summary['hidden'] = t('Visible in outline');
    }
    else {
      $summary['hidden'] = '<span class="warning">' . t('Not visible in outline') . '</span>';
    }

    // Required.
    if ($options['required']) {
      $summary['required'] = t('Completion required');
      if ($options['skippable']) {
        $summary['skippable'] = '<span class="warning">' . t('Skippable') . '</span>';
      }
    }
    else {
      $summary['required'] = '<span class="warning">' . t('Completion not required') . '</span>';
    }

    // Instance edit link.
    $editUrl = $this->getEditUrl();
    if (!empty($editUrl)) {
      $text = t('Edit instance');
      $summary['instance'] = l($text, $editUrl, array('external' => TRUE, 'query' => drupal_get_destination()));
    }
    elseif ($this->isTemporary()) {
      $summary['instance'] = '<span class="warning">' . t('Save course to edit object') . '</span>';
    }

    // Required.
    if (!empty($options['delete'])) {
      $dest = "node/{$options['nid']}/course-object/nojs/{$uniqid}/restore";
      $text = t('Object will be removed from outline');
      $restore_text = t('Restore this object to the course outline.');
      if ($options['delete_instance']) {
        $text = t('Object will be removed from outline, and related instance(s) will be deleted');
        $restore_text = t('Restore this object and related instance(s) to the course outline.');
      }
      $restore = ctools_ajax_text_button(t('Restore'), $dest, $restore_text);
      $summary['delete'] = '<span class="error">' . $text . '</span>' . ' ' . $restore;
    }

    return $summary;
  }

  /**
   * Get all course object implementations of getOptionsSummary().
   */
  public function renderOptionsSummary() {
    ctools_include('plugins');
    $summary = $this->getOptionsSummary();

    // Get summaries from plugins, and merge into the summary.
    foreach (ctools_get_plugins('course', 'course_object_access') as $key => $plugin) {
      // @todo how do we get these?
    }

    // @todo make this a themeable function.
    //return theme('course_object_summary');
    $rendered_summary = '';
    if (!empty($summary)) {
      $rendered_summary = $html = '<div class="description">' . theme('item_list', array('items' => $summary)) . '</div>';
    }

    return $rendered_summary;
  }

  /**
   * Get options, with session options having precedence.
   */
  public function getOptions() {
    $options = parent::getOptions();
    $sessionDefaults = array();
    if (isset($options['nid']) && isset($options['coid']) && isset($_SESSION['course'][$options['nid']]['editing'][$options['coid']])) {
      $sessionDefaults += array_filter((array) $_SESSION['course'][$options['nid']]['editing'][$options['coid']], array($this, 'optionFilter'));
    }
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
  public final function takeCourseObject() {
    $_SESSION['course']['active'] = $this->getCourseNid();
    $_SESSION['course'][$this->getCourseNid()]['taking']['active'] = $this->getId();

    // Run access checks.
    if ($this->access('take')) {
      // Grant access to external course object.
      $this->grant();

      // Record start date.
      if (!$this->getFulfillment()->getOption('date_started')) {
        $this->getFulfillment()->setOption('date_started', REQUEST_TIME);
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
    return t('This should be overridden by the module to return course content.');
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
   * If this course object is required, can be it skipped?
   *
   * @return bool
   */
  public function isSkippable() {
    return (bool) $this->getOption('skippable');
  }

  /**
   * Is this object graded?
   *
   * Returning TRUE here will cause some more configurations to show on the
   * object's form.
   *
   * @return bool
   */
  function isGraded() {
    return FALSE;
  }

  /**
   * Get the user's status in this course object.
   *
   * This is how an object would notify the user why they cannot proceed to the
   * next step from the course outline. For example, if this was a quiz and
   * they failed, this should let them know.
   */
  public function getStatus() {

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
    return $this->setOption('instance', $id);
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
   * @param Course|int $course
   *   A Course or node ID.
   *
   * @return CourseObject
   */
  public function setCourse($course) {
    if (is_numeric($course)) {
      $this->setOption('nid', $course);
      $courseNode = node_load($course);
      $this->course = course_get_course($courseNode);
    }
    else {
      $this->course = $course;
      $this->setOption('nid', $course->getNode()->nid);
    }

    return $this;
  }

  /**
   * Get the Course that contains this CourseObject.
   *
   * @return Course
   */
  function getCourse() {
    if (!$this->course) {
      $nid = $this->getCourseNid();
      if ($nid) {
        $this->course = new Course(node_load($nid), $this->user);
      }
    }

    return $this->course;
  }

  /**
   * Get the module that provides this course object.
   *
   * @return string
   */
  function getModule() {
    return $this->getOption('module');
  }

  /**
   * Get the object component title for this course object.
   *
   * @return string
   */
  function getComponentName() {
    $handlers = course_get_handlers('object');
    return $handlers[$this->getModule()][$this->getComponent()]['name'];
  }

  /**
   * Get the object component for this course object.
   *
   * @return string
   */
  function getComponent() {
    return $this->getOption('object_type');
  }

  /**
   * Set the module that provides this course object.
   *
   * @return CourseObject
   */
  function setModule($module) {
    return $this->setOption('module', $module);
  }

  /**
   * Set the object component for this course object.
   *
   * @return CourseObject
   */
  function setComponent($component) {
    return $this->setOption('object_type', $component);
  }

  /**
   * Set the internal course object ID.
   *
   * @param int $coid
   *   ID of the course object.
   */
  function setId($coid) {
    return $this->setOption('coid', $coid);
  }

  /**
   * Set the user fulfilling/creating this course object.
   */
  function setUser($user) {
    $this->user = $user;
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
    $object_info = course_get_handlers('object');

    // If title is not specified, set title from component.
    if (!$this->getOption('title')) {
      // Get the component name from object info.
      $title = $object_info[$this->getOption('module')][$this->getOption('object_type')]['name'];
      $this->setOption('title', $title);
    }

    return $this->getOption('title');
  }

  /**
   * Give the course object a chance do asynchronous polling and set completion
   * on demand.
   *
   * Useful for external objects.
   */
  function poll() {

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
    return array(
      'default' => array(
        'title' => 'Overview',
      ),
    );
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
    if ($key == 'default') {
      return array(
        'title' => 'Overview',
        'content' => views_embed_view('course_object_report', 'default', $this->getCourseNid(), $this->getId()),
      );
    }
  }

  function freeze() {

  }

  function thaw($ice) {

  }

  /**
   * Returns an translated error message if this object has issues with cloning.
   *
   * @return mixed
   *   TRUE if cloning is supported, FALSE if cloning is not supported. A string
   *   if the object can clone but with caveats.
   */
  function getCloneAbility() {
    return t('The course object %object cannot be cloned. A new instance will be created.', array('%object' => $this->getTitle()));
  }

  /**
   * Let objects create their instances before saving the course object.
   */
  public function save() {
    // If there is no title, set it.
    $this->getTitle();

    if ($ice = $this->getOption('freeze')) {
      // Found frozen data.
      $this->setInstanceId($this->thaw($ice));
      $this->setOption('freeze', NULL);
    }

    // If there is no instance ID, create one.
    if (!$this->getInstanceId()) {
      $this->create();
    }

    // Set the ID to NULL because this is a temporary course object being
    // created for the first time.
    if (strpos($this->getId(), 'course_object_') !== FALSE) {
      $this->setId(NULL);
    }

    return parent::save();
  }

  /**
   * Remove any records associated with this course object for the user.
   *
   * For example, deleting quiz results when a user is removed from the course.
   */
  function unEnroll() {

  }

  /**
   * Checks the temporary status of a course object.
   */
  function isTemporary() {
    return strpos($this->getId(), 'course_object_') === 0;
  }

  /**
   * Return the number of occurances that can be in a course at the same time.
   * For example, the design of the Certificate module can only have 1 set of
   * mappings per node. The same goes for Course Credit. We may also want a
   * course object that can only be added twice (for example, a before/after
   * comparison).
   *
   * This method is static because we might have to call it without an object
   * being instantiated.
   */
  public static function getMaxOccurences() {
    return FALSE;
  }

  /**
   * Set the context of which course this course object belongs to.
   *
   * The return parameters should be compliant with course_determine_context().
   */
  public static function context() {

  }

}

/**
 * Parent class for course object fulfillment.
 *
 * Represents the fulfillment record in the database.
 *
 */
class CourseObjectFulfillment extends CourseHandler {

  private $courseObject;

  /**
   * Construct the fulfillment object.
   *
   * A CourseObject and user are required to construct a fulfillment object.
   *
   * @param CourseObject $courseObject
   * @param Object $user
   */
  function __construct(CourseObject $courseObject, $user) {
    // Set storage.
    $this->entity_type = 'course_object_fulfillment';
    $this->table = 'course_outline_fulfillment';
    $this->primaryKey = 'cofid';
    $this->serializedField = 'info';

    $this->courseObject = $courseObject;
    $sql = "SELECT * FROM {course_outline_fulfillment} WHERE coid = %d AND uid = %d";
    $fulfillment = db_query("SELECT * FROM {course_outline_fulfillment} WHERE coid = :coid AND uid = :uid", array(':coid' => $this->courseObject->getId(), ':uid' => $user->uid))->fetch(PDO::FETCH_ASSOC);

    $this->setOptions($fulfillment);

    // Set course object ID.
    $this->setOption('coid', $this->courseObject->getId());
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
   *
   * @return CourseObjectFulfillment
   */
  function setComplete($complete = 1) {
    if (!$this->getOption('date_completed')) {
      $this->setOption('date_completed', REQUEST_TIME);
    }

    return $this->setOption('complete', $complete);
  }

  /**
   * Set this fulfillment's grade.
   *
   * @param float $grade
   *
   * @return CourseObjectFulfillment
   */
  function setGrade($grade) {
    return $this->setOption('grade_result', $grade);
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
    entity_delete('course_object_fulfillment', $this->getId());
  }

  /**
   * Track course after saving fulfillment.
   */
  public function save() {
    if (course_enrollment_check($this->getCourseObject()->getCourseNid(), $this->getCourseObject()->getCourse()->getUser()->uid)) {
      parent::save();
      $this->getCourseObject()->getCourse()->track();
    }
  }

}

/**
 * A course object that uses a node as a base.
 */
abstract class CourseObjectNode extends CourseObject {

  protected $node;

  public function __construct($object, $user = NULL, $course = NULL) {
    // Pass configuration to parent.
    parent::__construct($object, $user, $course);
    $this->node = node_load($this->getInstanceId());
  }

  /**
   * Course context handler callback.
   */
  public static function context() {
    // Get node from URL.
    if (arg(0) == 'node' && $node = node_load(arg(1))) {

      // This node might not be in a course, so let's check for related nodes.
      $instances = static::getNodeInstances($node);
      if (!empty($instances)) {
        $node = node_load($instances[0]);
      }

      if (isset($node->nid)) {
        return array(
          'object_type' => $node->type,
          'instance' => $node->nid,
        );
      }
    }
  }

  /**
   * When passed a node, this method should return the "parent" nodes that are
   * contained in a course outline.
   *
   * For example, if the passed node was a question in a quiz, all the quiz node
   * IDs should be returned.
   */
  public static function getNodeInstances($node) {
    return array();
  }

  public function hasNodePrivacySupport() {
    return module_exists('content_access') && module_exists('acl');
  }

  /**
   * Return a list of valid node types.
   *
   * @return array
   *   An array keyed by machine name of a node type.
   *   return array(
   *     'my_event' => t('My event content type'),
   *   );
   */
  public abstract function getNodeTypes();

  /**
   * Simple node course object behavior is to just redirect to the node.
   */
  public function getTakeType() {
    return 'redirect';
  }

  public function getTakeUrl() {
    if ($this->getInstanceId()) {
      return url("node/{$this->node->nid}");
    }
  }

  public function getEditUrl() {
    if ($this->getInstanceId()) {
      return url("node/{$this->node->nid}/edit");
    }
  }

  /**
   * Create a node and set it as this course object's node.
   *
   * @param stdClass $node
   *   A node to be processed for creation, or none to create a generic node. If
   *   a node is provided, it must have at least a type.
   */
  public function create($node = NULL) {
    if (!$node) {
      $node = new stdClass;
      $node->type = $this->getOption('node_type');
    }
    $node->title = $this->getTitle();
    $node->uid = $this->user->uid;
    $node->language = LANGUAGE_NONE;
    node_object_prepare($node);
    node_save($node);
    $this->setNode($node);
  }

  /**
   * Set the node and instance ID (node ID) of this CourseObjectNode.
   *
   * @param mixed $node
   *   A node or node ID.
   */
  public function setNode($node) {
    $this->node = $node;
    $this->setInstanceId($this->node->nid);
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

    $options = array_intersect_key(node_type_get_names(), $this->getNodeTypes());
    $defaults['node_type'] = key($options);

    $defaults['use_node_title'] = 0;

    return $defaults;
  }

  public function optionsForm(&$form, &$form_state) {
    parent::optionsForm($form, $form_state);

    $form['node'] = array(
      '#type' => 'fieldset',
      '#title' => t('Content'),
      '#description' => ('Settings for course object content.'),
      '#group' => 'course_tabs',
      '#weight' => 2,
    );

    $config = $this->getOptions();

    if (!$this->getInstanceId()) {
      $types = drupal_map_assoc($this->getNodeTypes());
      $options = array_intersect_key(node_type_get_names(), $types);

      $form['node']['use_existing_node'] = array(
        '#type' => 'checkbox',
        '#title' => t('Use existing content'),
        '#default_value' => 0,
      );

      $form['node']['node_type'] = array(
        '#title' => t('Create node'),
        '#type' => 'select',
        '#options' => $options,
        '#description' => t('Selecting a node type will automatically create this node and link it to this course object.'),
        '#default_value' => $config['node_type'],
        '#states' => array(
          'visible' => array(
            ':input[name="use_existing_node"]' => array('checked' => FALSE),
          ),
        ),
      );
      if (count($options) > 1) {
        $form['node']['node_type']['#required'] = TRUE;
      }
    }

    $form['node']['instance'] = array(
      '#title' => t('Existing node'),
      '#autocomplete_path' => 'course/autocomplete/node/' . implode(',', $this->getNodeTypes()),
      '#type' => 'textfield',
      '#description' => t('Use an existing node instead of creating a new one.'),
      '#default_value' => !empty($this->node->nid) ? check_plain($this->node->title) . " [nid: {$this->node->nid}]" : NULL,
      '#states' => array(
        'visible' => array(
          ':input[name="use_existing_node"]' => array('checked' => TRUE),
        ),
      ),
    );

    $form['node']['use_node_title'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use existing title'),
      '#description' => t("Use the referenced content's title as this course object's title."),
      '#default_value' => $config['use_node_title'],
    );

    $form['node']['private'] = array(
      '#title' => t('Private'),
      '#description' => $this->hasNodePrivacySupport() ? t('This content will not be available to users who are not enrolled in this course.') : t('You must enable content_access and acl in order to restrict course content to users who are enrolled in this course.'),
      '#type' => 'checkbox',
      '#default_value' => $config['private'],
      '#disabled' => !($this->hasNodePrivacySupport()),
    );

    $nid = $this->getInstanceId();
    if ($nid) {
      $node = node_load($nid);
      $link = l(t("'%title' [node id %nid]", array('%title' => $node->title, '%nid' => $node->nid)), "node/$node->nid", array('attributes' => array('target' => '_blank', 'title' => t('Open in new window')), 'html' => TRUE));
      $form['node']['instance']['#description'] = t('Currently set to !link', array('!link' => $link));
    }

    // Block deletion of self-referencing node instances.
    if (!empty($config['instance']) && ($config['instance'] == $this->getCourseNid())) {
      $form['delete']['delete_instance']['#disabled'] = TRUE;
      $form['delete']['delete_instance']['#description'] = t('You cannot delete this instance, as it references this Course.');
    }
  }

  /**
   * Validate the options form. Check the node type.
   */
  public function optionsValidate(&$form, &$form_state) {
    if (isset($form_state['values']['node_type']) && empty($form_state['values']['node_type'])) {
      form_set_error('node_type', t('Please select a node type.'));
    }
  }

  public function optionsSubmit(&$form, &$form_state) {
    $nid = $form_state['values']['instance'];

    if (!is_numeric($nid)) {
      if (preg_match('/^(?:\s*|(.*) )?\[\s*nid\s*:\s*(\d+)\s*\]$/', $nid, $matches)) {
        $nid = $matches[2];
      }
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
      $type_name = $this->getComponent();
      $acl_var_name = 'content_access_' . $type_name;
      $settings = variable_get($acl_var_name, array());

      if (!isset($settings['per_node']) || !$settings['per_node']) {
        $warnings[] = t('%t is set to Private, but the content type %c does not have access control lists enabled. Users will not be able to acces this content. Please visit !l to set up content access settings.', array(
          '%t' => $this->getTitle(),
          '%c' => $this->getComponent(),
          '!l' => l('Access control', "admin/structure/types/manage/{$this->getComponent()}/access"),
        ));
      }
    }
    return $warnings;
  }

  /**
   * Grant access to course content before going to it.
   */
  function grant() {
    if ($this->hasNodePrivacySupport()) {
      if ($this->getOption('private')) {
        $uid = $this->user->uid;
        module_load_include('inc', 'content_access', 'content_access.admin');
        $acl_id = content_access_get_acl_id($this->node, 'view');
        acl_add_user($acl_id, $uid);
        acl_node_add_acl($this->node->nid, $acl_id, 1, 0, 0, content_access_get_settings('priority', $this->node->type));
        node_access_acquire_grants($this->node);
      }
    }
  }

  /**
   * Duration expired (or something) - CourseObject is telling us so.
   */
  function revoke() {
    if ($this->hasNodePrivacySupport()) {
      if ($this->getOption('private')) {
        $uid = $this->user->uid;
        module_load_include('inc', 'content_access', 'content_access.admin');
        $acl_id = content_access_get_acl_id($this->node, 'view');
        acl_remove_user($acl_id, $uid);
        node_access_acquire_grants($this->node);
      }
    }
  }

  /**
   * On object write, set privacy on this node.
   */
  function save() {
    parent::save();
    if ($this->hasNodePrivacySupport() && $this->getOption('private')) {
      // Ensure that per-node access is enabled.
      $type_settings = content_access_get_settings('all', $this->node->type);
      $type_settings['per_node'] = 1;
      content_access_set_settings($type_settings, $this->node->type);

      // Remove "view" permissions to everyone on this node.
      $settings = content_access_get_per_node_settings($this->node);
      $settings['view'] = array();
      content_access_save_per_node_settings($this->node, $settings);

      // Resave node to update access.
      node_access_acquire_grants($this->node);
    }
  }

  /**
   * Freeze data to persist over cloning/exporting.
   * @return array
   *   An array of data to be frozen.
   */
  function freeze() {
    if ($this->node->nid != $this->getCourse()->getNode()->nid) {
      // Don't freeze the course, if this course is part of the objects.
      $ice = new stdClass;
      $ice->node = $this->node;
      return $ice;
    }
  }

  /**
   * Thaw data frozen from an earlier export/clone.
   *
   * @param array $data
   *   Unfrozen data.
   *
   * @return int
   *   The new instance ID.
   */
  function thaw($ice) {
    $this->node = $ice->node;
    unset($this->node->nid);
    unset($this->node->vid);
    node_save($this->node);
    return $this->node->nid;
  }

  function getCloneAbility() {
    return t('%object will be cloned as a node. Results may vary.', array('%object' => $this->getTitle()));
  }

  /**
   * Get the object title, or return this object's node's title if the option
   * is set.
   */
  function getTitle() {
    if ($this->getOption('use_node_title') && $this->node) {
      return $this->node->title;
    }
    else {
      return parent::getTitle();
    }
  }

  function getNode() {
    return $this->node;
  }

  function getOptionsSummary() {
    $summary = parent::getOptionsSummary();
    if ($this->getOption('instance') == $this->getCourseNid()) {
      // Don't show the edit instance link when the instance is
      // self-referencing.
      unset($summary['instance']);
    }
    return $summary;
  }

  /**
   * When setting our instance ID, also set the node.
   */
  function setInstanceId($id) {
    parent::setInstanceId($id);
    $this->node = node_load($id);
  }

}

class CourseObjectBroken extends CourseObject {

  function take() {
    return t('This course object is misconfigured. Please contact the administrator.');
  }

}
