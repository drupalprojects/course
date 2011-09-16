<?php

/**
 * @file course.drupal.inc
 * Support for Drupal as an LMS (integrating Quiz, Scorm)
 */

/**
 * Implementation of hook_lms_info().
 */
function course_lms_info() {
  if (!variable_get('course_disable_builtin_lms', 0)) {
    return array(
      'drupal' => array(
        'name' => t('Drupal'),
        'description' => t('A course with Drupal learning objects.'),
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
 * Implementation of hook_lms_settings_form().
 */
function course_lms_settings_form($lapp_id) {
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
 * Implementation of hook_lms_status().
 */
function course_lms_status($lapp_id) {
  if ($lapp_id == 'drupal') {
    return TRUE;
  }
  if ($lapp_id == 'none') {
    return TRUE;
  }
}

/**
 * Implementation of hook_lms_install().
 *
 * Enable modules necessary for this LMS to function.
 */
function course_lms_install($lapp_id) {
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
 * Pre-set quiz settings, etc...
 */
function course_lms_postinstall() {
}

/**
 * Implementation of hook_lms_take_course().
 */
function course_lms_take_course($key, $node) {
  if ($key == 'drupal') {
    return course_get_outline($node);
  }
}

/**
 * Implementation of hook_lms_edit_course().
 */
function course_lms_edit_course($key, $node) {
  if ($key == 'drupal') {

  }
}
