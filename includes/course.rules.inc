<?php

/**
 * Implements hook_rules_event_info().
 */
function course_rules_event_info() {
  $items = array(
    'course_enrollment_insert' => array(
      'label' => t('After a user is enrolled in a course'),
      'group' => t('Course'),
      'variables' => array(
        'node' => array(
          'label' => t('Node'),
          'type' => 'node',
        ),
        'user' => array(
          'label' => t('User'),
          'type' => 'user',
        ),
        'course_enrollment' => array(
          'label' => t('Enrollment'),
          'type' => 'course_enrollment',
        ),
      ),
    ),
    'course_enrollment_delete' => array(
      'label' => t('After a user is unenrolled from a course'),
      'group' => t('Course'),
      'variables' => array(
        'node' => array(
          'label' => t('Node'),
          'type' => 'node',
        ),
        'user' => array(
          'label' => t('User'),
          'type' => 'user',
        ),
        'course_enrollment' => array(
          'label' => t('Enrollment'),
          'type' => 'course_enrollment',
        ),
      ),
    ),
  );

  return $items;
}
