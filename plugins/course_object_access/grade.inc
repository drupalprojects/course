<?php

$plugin = array(
  'title' => t('Grade'),
  'description' => t('Controls access by grade'),
  'handler' => array(
    'class' => 'CourseObjectAccessGrade',
  ),
);

class CourseObjectAccessGrade extends CourseObjectAccess {

  public function optionsDefinition() {
    $defaults = parent::optionsDefinition();

    $defaults += array(
      'course_grade' => NULL,
      'course_grade_hidden' => NULL,
      'conditional_object' => NULL,
    );

    return $defaults;
  }

  function take() {
    $config = $this->getOptions();
    if ($config['course_grade']) {
      if ($config['conditional_object']) {
        // Check object grade.
        $target = course_object_load($config['conditional_object']);
        if ($config['course_grade'] > $target->getOption('grade_result')) {
          $this->getCourseObject()->setAccessMessage('grade', t('You must have a grade of at least @grade% in %object to take this activity.', array(
            '@grade' => $this->getOption('course_grade'),
            '%object' => $target->getTitle(),
          )));
          return FALSE;
        }
      }

      // Check course grade.
      if ($config['course_grade'] > $this->getCourseObject()->getCourse()->getTracker()->getOption('grade_result')) {
        $this->getCourseObject()->setAccessMessage('grade', t('You must have a grade of at least @grade% to take this activity.', array('@grade' => $this->getOption('course_grade'))));
        return FALSE;
      }
    }
  }

  function see() {
    if (!$this->take() && $this->getOption('course_grade_hidden')) {
      return FALSE;
    }
  }

  function view() {
    return $this->take();
  }

  function optionsForm(&$form, &$form_state) {
    $config = $this->getOptions();

    $form['course_grade'] = array(
      '#title' => t('Grade required'),
      '#type' => 'textfield',
      '#size' => 4,
      '#description' => t('The user will not be able to access this object until this course grade is met.'),
      '#default_value' => $config['course_grade'],
    );

    $options = $this->getObjectOptions();
    $options[0] = '(this course)';
    $form['conditional_object'] = array(
      '#title' => t('Grade to check'),
      '#type' => 'select',
      '#options' => $options,
      '#description' => t('This grade will be checked.'),
      '#default_value' => $config['conditional_object'],
    );

    $form['course_grade_hidden'] = array(
      '#title' => t('Hide until grade met'),
      '#description' => t('The user will not see this object until the grade is met.'),
      '#type' => 'checkbox',
      '#default_value' => $config['course_grade_hidden'],
    );

    return $form;
  }

}
