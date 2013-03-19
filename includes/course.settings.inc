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
        l(t('Edit'), "admin/config/course/{$package_key}", array('query' => drupal_get_destination())),
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
 * Form for configuring user types.
 *
 * @todo move to entities or something, to store information about the user's
 * enrollment. maybe registration?
 */
function course_user_type_settings_form() {
  $form = array();

  $form['header'] = array(
    '#value' => t('User types allow a user to select their type only for the enrollment duration of taking the course. For example, other modules may implement a mapping from the user type to course credit or a certificate while leaving a global profile field unchanged.'),
  );

  $form['course_user_types_enabled'] = array(
    '#title' => t('Use course enrollment user types'),
    '#type' => 'checkbox',
    '#validate' => 'course_user_type_settings_form_validate',
    '#default_value' => variable_get('course_user_types_enabled', ''),
  );

  $form['course_user_types_field'] = array(
    '#title' => t('Profile field to pull types from'),
    '#type' => 'select',
    '#default_value' => variable_get('course_user_types_field', ''),
    '#options' => course_cck_kv_fields(),
  );

  $form['course_user_types'] = array(
    '#title' => t('Additional user types'),
    '#type' => 'textarea',
    '#rows' => 5,
    '#validate' => 'course_user_type_settings_form_validate',
    '#default_value' => variable_get('course_user_types', ''),
    '#description' => t('Input course enrollment user types in the format "key|value", one per line.'),
  );

  return system_settings_form($form);
}

/**
 * Validate the user input for enrollment types.
 */
function course_user_type_settings_form_validate(&$form, &$form_state) {
  $lines = explode("\n", trim($form_state['values']['course_user_types']));
  foreach ($lines as $line) {
    if (empty($line)) {
      return;
    }
    $kv = explode('|', $line);
    if (count($kv) != 2) {
      form_set_error('course_user_types', t('Each user type must be in key|value format.'));
    }
  }
}

/**
 * Get an array of field options that can be used as course enrollment types.
 */
function course_cck_kv_fields() {
  $field_options = array();

  $field_options[] = '- none -';

  if (module_exists('content')) {
    foreach (content_fields() as $key => $field) {
      // If field isn't a profile field with allowed values, do not allow it to be selectable. It wouldn't work anyways.
      if (optionwidgets_options($field)) {
        $field_options[$key] = $field['widget']['label'];
      }
    }
  }

  return $field_options;
}
