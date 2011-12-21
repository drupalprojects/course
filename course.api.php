<?php

/**
 * @file
 * This file contains documentation about hooks invoked by course module.
 *
 * @todo until we finish cleaning up all files and submodules, grep through
 * everything for 'drupal_alter', 'module_invoke_all', 'module_invoke', and
 * 'module_implements'.
 */

/**
 * @defgroup course_hooks Course's hooks
 * @{
 * Hooks that can be implemented by other modules to implement the Course API.
 */

////////////////////////////////////////////////////////////////////////////////
// Strongly recommended hooks.

/**
 * Hook allowing modules to provide lms types to course module.
 *
 * @return array
 *   An array of LMS declarations, keyed by LMS type, containing:
 *   - name: The LMS label.
 *   - description: The LMS description.
 *   - module: the implementing module. @todo remove this!
 *   - install url: optional LMS installation URL.
 *   - weight: the order of LMS selection appearance. @todo maybe remove this?
 *
 * @see course_available_lms()
 *
 * @todo consider globally replacing 'lms' (learning management system) with
 * 'lapp' (learning application), since normally course module is used to build
 * a Drupal-based learning management system.
 *
 * Required course submodule hook.
 */
function hook_lms_info() {
  // @todo add an even more generic example.
  if (!variable_get('course_disable_builtin_lms', 0)) {
    return array(
      'drupal' => array(
        'name' => 'Drupal',
        'description' => 'Provides courses based on Drupal Quiz learning objects.',
        'module' => 'course',
        'install url' => '',
        'weight' => 5,
      ),
      'none' => array(
        'name' => 'Placeholder',
        'description' => 'A course with no course objects.',
        'module' => 'course',
        'weight' => 10,
      ),
    );
  }
}

/**
 * Hook allowing course modules to extend the course settings form.
 *
 * @todo UX review of course settings.
 */
function hook_lms_settings_form($lapp_id) {
  $form = array();

  switch ($lapp_id) {
    case 'drupal':
      $form['course_something']['#type'] = 'radios';
      $form['course_something']['#options'] = array('Something', 'Something else');
      $form['course_something']['#default_value'] = variable_get('course_something', '');
      break;
  }

  return system_settings_form($form);
}

/**
 * Defines the installation status of an LMS.
 *
 * @param $lapp_id
 *   The learning application ID.
 *
 * @return bool
 *   The LMS installation status.
 */
function hook_lms_status($lapp_id) {
  if ($lapp_id == 'drupal') {
    $modules = _course_required_modules();
    foreach ($modules as $module) {
      if (!module_exists($module)) {
        return FALSE;
      }
    }
    return TRUE;
  }
  if ($lapp_id == 'none') {
    return TRUE;
  }
}

/**
 * Hook allowing lms include files to list other modules necessary for this LMS
 * LMS to function properly. The only reason we have this was a decision to use
 * inc files for LMS types rather than modules. Otherwise we would use Drupal's
 * built-in module dependency system.
 */
function hook_lms_install($lapp_id) {
  // @todo add an even more generic example.
  if ($lapp_id == 'drupal') {
    $out = '<h3>' . 'Module status' . '</h3>';
    $headers = array('Module', 'Status');
    $modules = _course_required_modules();
    foreach ($modules as $module) {
      if (module_exists($module)) {
        $rows[] = array($module, 'OK');
      }
      else {
        $rows[] = array($module, 'Not installed');
      }
    }
    return $out . theme_table($headers, $rows);
  }
}

/**
 * Hook allowing modules to run operations after installation, such as pre-set
 * quiz settings, etc.
 */
function hook_lms_postinstall() {
  // @todo document this.
}

/**
 * Modules declaring LMS types must return takecourse HTML through this hook.
 *
 * @param $key string
 *   The LMS key as defined in hook_lms_info().
 * @param $node object
 *   The course node.
 *
 * @see course_take_course()
 *   Useses this hook to determine if any LMS has been installed.
 * @see hook_lms_info()
 *   Defines the need for this hook.
 * @see course_lms_take_course()
 *   Implements this hook.
 *
 * @return string
 *   HTML output for the takecourse page.
 */
function hook_lms_take_course($key, $node) {
  // Example from course_lms_take_course().
  if ($key == 'drupal') {
    return course_get_outline($node);
  }
}

/**
 * @todo explain this hook.
 */
function hook_lms_edit_course($key, $node) {
  // @todo add an even more generic example.
  if ($key == 'drupal') {
    // Do something.
  }
}

/**
 * Expose objects to Course.
 *
 * Implementations should return a keyed array of course objects.
 *
 * @return array
 *   An array of course objects. The course object list is an associative array
 *   where the key is the machine name of the object. Each course object may
 *   contain the following key-value pairs:
 *     - 'title': The name to appear in object add forms
 *     - 'suggested title': The title to show by default in the workflow
 *     - 'graded': True or false, can this type be graded
 *     - 'class': The course object's class
 *     - 'description': A brief description of the course object.
 */
function hook_course_object_info() {
  // Example from course_quiz module.
  return array(
    'quiz' => array(
      'title' => 'Drupal Quiz',
      'suggested title' => 'Quiz',
      'graded' => TRUE,
      'class' => 'QuizCourseObject',
      'description' => 'A drupal quiz as a course object.',
    ),
  );
}

/**
 * The API for creating course objects on the fly.
 *
 * By "presave" we mean the course object record has not yet been saved to the
 * database.
 *
 * @return int
 *   An instance ID (node, external object ID, etc.).
 */
function hook_course_object_api($node, $op, $object) {
  // @todo add an even more generic example.
  if ($op == 'presave') {
    switch ($object['requirement_component']) {
      case 'quiz':
        if ($object['instance']) {
          $quiz = node_load($object['instance']);
        }
        else {
          $quiz = new stdClass;
        }
        $quiz->auto_created = TRUE;
        $quiz->type = 'quiz';
        $quiz->title = $object['title'];
        $quiz->uid = $node->uid;
        node_save($quiz);
        return $quiz->nid;
    }
  }
}

/**
 * Allow external LMSs to add a unique identifier to the drupal course object on
 * save and update.
 *
 * @param $node object
 *   The course node.
 *
 * @return int
 *   An enternal LMS course identifier.
 *
 * @see course_nodeapi()
 */
function hook_course_create_external($node) {
  // Example from course_moodle module, returning a numeric moodle course id.
  return course_moodle_api_course_post($node);
}

////////////////////////////////////////////////////////////////////////////////
// Optional hooks.

/**
 * Hook allowing modules to determine course take button display.
 */
function hook_course_has_takecourse($node, $user) {
  // @todo document this.
}

/**
 * @todo explain this hook.
 */
function hook_course_has_settings($node, $user) {
  // @todo document this.
}

/**
 * Hook allowing modules to determine if this course should be restricted.
 */
function hook_can_take_course($node, $user) {
  // @todo document this.
}

/**
 * Hook allowing other course modules to know about the recently created course.
 *
 * @param $node
 *   The fully loaded node object.
 *
 * @param $op
 *   Accepts $op values from hook_nodeapi().
 */
function hook_course_nodeapi_extra($node, $op) {
  // @todo document this.
}

/**
 * @todo explain this hook.
 */
function hook_course_button($node) {
  // @todo document this.
}

/**
 * Hook notifying other modules about a course enrollment.
 *
 * @todo document params.
 */
function hook_course_enrol($node, $user, $from, $code, $status) {
  // @todo document this.
}

/**
 * Hook notifying other modules after course unenrollment.
 *
 * @todo document params.
 */
function hook_course_unenrol($node, $user) {
  // @todo document this.
}

/**
 * Hook allowing other modules to force the take course button to show.
 */
function hook_course_show_button_alter(&$success, $node) {
  // @todo document this.
}

/**
 * Hook allowing other modules to alter course report.
 *
 * @param $entry object
 *   The report entry containing:
 *   - nid: the node id.
 *   - uid: the user id.
 *   - data: an array containing.
 *     - user: the serialized user object at the time of entry.
 *     - profile: the serialized user profile at the time of entry.
 *   - updated: the entry time.
 * @param $account object
 *   The fully loaded report user.
 * @param $old
 *   The currently saved version of the user's report for a course.
 *
 * @see course_report_save()
 */
function hook_course_report_alter(&$entry, $account, $old) {
  // @todo add example.
}

/**
 * @}
 */
