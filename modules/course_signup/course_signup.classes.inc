<?php

class CourseObjectSignup extends CourseObjectNode {

  public function create() {
    if (!$this->getOption('instance')) {
      $this->setOption('instance', $this->getCourseNid());
    }
  }

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

  public function optionsForm(&$form, &$form_state) {
    parent::optionsForm($form, $form_state);

    $form['instance']['#title'] = t('Signup to check');
    $form['instance']['#description'] .= '</br>' . t('Check a different signup-enabled product for attendance. Defaults to this course.');
    $form['instance']['#autocomplete_path'] = 'course-signup/autocomplete';
  }

  /**
   * Set a specialized instance ID.
   *
   * Either the course node ID, or another signup node ID.
   */
  public function optionsSubmit(&$form, &$form_state) {
    if (!$form_state['values']['instance']) {
      $form_state['values']['instance'] = $this->getCourseNid();
    }
    parent::optionsSubmit($form, $form_state);
  }

  public function getReports() {
    return array(
      'attendance' => array(
        'title' => 'Attendance',
      ),
    );
  }

  public function getReport($key) {
    module_load_include('inc', 'signup', 'includes/node_admin');
    switch ($key) {
      case 'attendance':
        return array(
          'title' => t('Attendance'),
          'content' => signup_node_admin_page($this->node),
        );
    }
  }

}
