<?php

class CourseObjectCertificate extends CourseObject {

  public function take() {
    return l('Download certificate', "node/" . $this->getCourseNid() . "/certificate");
  }

  public function optionsDefinition() {
    return array(
      'certificate_node' => 1,
    );
  }

  public function optionsForm(&$form, &$form_state) {
    parent::optionsForm($form, $form_state);
    $options = $this->getOptions();

    $form['certificate_node'] = array(
      '#title' => 'Use node settings from Certificate',
      '#type' => 'checkbox',
      '#default_value' => $options['certificate_node'],
      '#description' => 'This will direct the user to node/' . $this->getCourseNid() . '/certificate',
    );
  }

  public function getTakeType() {
    return 'content';
  }

}
