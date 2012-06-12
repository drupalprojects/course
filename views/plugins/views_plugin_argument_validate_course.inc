<?php

/**
 * Validate whether an argument is a course.
 */
class views_plugin_argument_validate_course extends views_plugin_argument_validate {

  function validate_form(&$form, &$form_state) {

    $form[$this->option_name] = array(
      '#type' => 'checkbox',
      '#title' => t('Is Live Course'),
      '#default_value' => $this->get_argument(),
      '#description' => t('Is this course a Live course?'),
      '#process' => array('views_process_dependency'),
      '#dependency' => array('edit-options-validate-type' => array($this->id)),
    );

    $this->check_access($form);
  }

  function validate_argument($argument) {
    $node = node_load($argument);
    if ($this->argument->options[$this->option_name]) {
      if ($node->field_course_type[0]['value'] != 'live') {
        return FALSE;
      }
    }
    $this->argument->validated_title = check_plain($node->title);
    return course_node_is_course($node);
  }

}
