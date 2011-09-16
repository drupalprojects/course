<?php

/**
 * @file
 * This file contains documentation about hooks invoked by course module.
 *
 * @todo untill we finish cleaning up all files and submodules, grep through
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
 * Required course submodule hook.
 */
function hook_lms_info() {
  // @todo add an even more generic example.
  if (!variable_get('course_disable_builtin_lms', 0)) {
    return array(
      'drupal' => array(
        'name' => t('Drupal'),
        'description' => t('Provides courses based on Drupal Quiz learning objects.'),
        'module' => 'course',
        'install url' => '',
        'weight' => 5,
      ),
      'none' => array(
        'name' => t('Placeholder'),
        'description' => t('A course with no course objects.'),
        'module' => 'course',
        'weight' => 10,
      ),
    );
  }
}

/**
 * Hook allowing course modules to extend the course settings form.
 */
function hook_lms_settings_form($lapp_id) {
  $form = array();

  switch ($lapp_id) {
    case 'drupal':
      $form['course_something']['#type'] = 'radios';
      $form['course_something']['#options'] = array('Something','Something else');
      $form['course_something']['#default_value'] = variable_get('course_something','');
      break;
  }

  return system_settings_form($form);
}

/**
 * @todo explain this hook.
 */
function hook_lms_status($lapp_id) {
  // @todo add an even more generic example.
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
 * @todo explain this hook.
 */
function hook_lms_take_course($key, $node) {
  // @todo add an even more generic example.
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
  // @todo add an even more generic example.
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
 * @todo explain this hook.
 */
function course_create_external($node) {
  // @todo document this.
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
function hook_course_show_button_alter($success, $node) {
  // @todo document this.
}

/**
 * @}
 */
