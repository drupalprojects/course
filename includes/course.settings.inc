<?php

/**
 * @file
 * Administrative settings for Course module.
 */

/**
 * Menu callback: Define the Course settings form.
 *
 * @param array $settings
 *   (optional) The loaded settings handler.
 */
function course_settings_overview($form, &$form_state) {
  // Course settings handler forms. This allows Course to ensure consistent
  // placement of forms from modules that define settings handlers through
  // hook_course_handlers().
  $form['course_extensions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings overview'),
  );
  $modules = course_get_handlers('settings');
  $handlers = array();
  foreach ($modules as $settings) {
    if (is_array($settings)) {
      $handlers += $settings;
    }
  }
  $header = array(t('Package'), t('Description'), t('Operations'));
  $rows = array();
  foreach ($handlers as $handler_key => $handler_info) {
    // Generate a list of each handler category.
    $package_key = isset($handler_info['package']) ? $handler_info['package'] : $handler_key;
    $package_info = $handlers[$package_key] ? $handlers[$package_key] : $handler_info;

    if (!isset($package_set[$package_key])) {
      $rows[] = array(
        $package_info['name'],
        $package_info['description'],
        l(t('Edit'), "admin/course/{$package_key}", array('query' => drupal_get_destination())),
      );
      $package_set[$package_key] = TRUE;
    }
  }
  $form['course_extensions']['course_extensions_table'] = array('#markup' => theme('table', array('header' => $header, 'rows' => $rows)));

  return $form;
}

/**
 * Appearance settings form callback.
 */
function course_settings_appearance_form() {
  $form = array();

  $form['course_takecourse_tab_display'] = array(
    '#title' => t('Show a "take course" tab on course nodes'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('course_takecourse_tab_display', 1),
  );

  $form['course_take_course_button_show'] = array(
    '#title' => t('Show "take course" button on'),
    '#type' => 'checkboxes',
    '#options' => array(
      'teaser' => t('Teaser'),
      'full' => t('Full'),
    ),
    '#default_value' => variable_get('course_take_course_button_show', array('full' => 'full')),
    '#description' => t('Check to show a "take course" button on courses.'),
  );

  $form['course_disable_regions'] = array(
    '#title' => t('Disable theme regions when taking a course'),
    '#type' => 'checkboxes',
    '#default_value' => variable_get('course_disable_regions', array()),
    '#options' => system_region_list(variable_get('theme_default', '')),
  );

  return system_settings_form($form);
}

/**
 * Element validator to make sure all entries are in key|val format.
 */
function course_check_keyval($element, &$form_state) {
  $val = trim($element['#value']);
  if ($val) {
    if (substr_count($val, "\n") + 1 != substr_count($val, '|')) {
      form_error($element, t('Please ensure all custom object fields are in the format key|value.'));
    }
  }
}

/**
 * Settings form for course enrollments.
 */
function course_enrollment_settings_form($form) {
  $form['header']['#markup'] = '<p>' . t('Here, you can manage the settings related to course enrollments.') . '</p>';
  $form['header']['#markup'] .= '<p>' . t('Fields added to the enrollment entity can optionally be displayed in a form to the user before enrollment.') . '</p>';
  return $form;
}

/**
 * Settings form for course reporting.
 */
function course_report_settings_form($form) {
  $form['header']['#markup'] = '<p>' . t('Here, you can manage the settings related to course progress and completion.') . '</p>';
  $form['header']['#markup'] .= '<p>' . t('Fields may be added to the course progress tracker entity for future functionality.') . '</p>';
  return $form;
}

/**
 * Settings form for course objects.
 */
function course_object_settings_form($form) {
  $form['header']['#markup'] = '<p>' . t('Here, you can manage the settings related to course objects.') . '</p>';
  $form['header']['#markup'] .= '<p>' . t('Fields added to the course object entity are included on the course object editing form, and may be used in course object theme hooks.') . '</p>';
  return $form;
}
