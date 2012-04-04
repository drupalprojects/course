<?php

class CourseObjectSignup extends CourseObject {

  public function getTakeType() {
    return 'content';
  }

  public function take() {
    if ($this->getFulfillment()->isComplete()) {
      return "You have been marked attended by an administrator.";
    }
    else {
      return "You must be marked attended by an administrator.";
    }
  }

  public function optionsDefinition() {
    $options = parent::optionsDefinition();
    $options['signup_this_course'] = NULL;
    $options['signup_nid'] = NULL;
    return $options;
  }

  public function optionsForm(&$form, &$form_state) {
    parent::optionsForm($form, $form_state);
    $defaults = $this->getOptions();
    ctools_add_js('dependent');
    ctools_include('dependent');

    $form['signup_this_course'] = array(
      '#title' => 'Check attendance for this course',
      '#type' => 'checkbox',
      '#default_value' => $defaults['signup_this_course'],
      '#description' => 'Check this course for attendance.'
    );

    $form['signup_nid'] = array(
      '#title' => t('Check a different signup'),
      '#type' => 'textfield',
      '#maxlength' => 512,
      '#description' => 'Check a different signup-enabled node for attendance.',
      '#autocomplete_path' => 'course-signup/autocomplete',
      '#process' => array('ctools_dependent_process'),
      '#dependency' => array(
        'edit-signup-this-course' => array(0),
      ),
    );

    return $form;
  }

  /**
   * Set a specialized instance ID.
   *
   * Either the course node ID, or another signup node ID.
   */
  public function optionsSubmit(&$form, &$form_state) {
    if ($form_state['values']['signup_this_course']) {
      $this->setInstanceId($this->getCourseNid());
    }
    parent::optionsSubmit($form, $form_state);
  }

}
