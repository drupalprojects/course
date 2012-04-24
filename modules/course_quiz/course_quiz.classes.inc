<?php

class CourseObjectQuiz extends CourseObjectNode {

  /**
   * Create the quiz node and set it as this object's instance.
   */
  function create() {
    global $user;
    if ($this->getInstanceId()) {
      $quiz = node_load($this->getInstanceId());
    }
    else {
      $quiz = new stdClass;
    }
    $quiz->auto_created = TRUE;
    $quiz->type = 'quiz';
    $quiz->title = $this->getTitle();
    $quiz->uid = $user->uid;
    $quiz->quiz_always = TRUE;
    $quiz->quiz_open = $quiz->quiz_close = array(
      'month' => date('m'),
      'day' => date('d'),
      'year' => date('Y'),
    );
    $quiz = (object) array_merge(_quiz_get_node_defaults(), (array) $quiz);
    node_save($quiz);
    $this->setInstanceId($quiz->nid);
  }

  function getTakeUrl() {
    return url("node/{$this->node->nid}/take");
  }

  /**
   * Marks a user's fulfillment record for this object complete if the user
   * passed the quiz.
   */
  function grade($user) {
    $nid = (int) $this->getInstanceId();

    $result = reset(quiz_get_score_data(array($nid), $user->uid));
    if ($result && ($result->percent_score >= $result->percent_pass)) {
      $this->getFulfillment()->setGrade($result->percent_score)->setComplete()->save();
    }
    else {
      $this->getFulfillment()->setGrade($result->percent_score)->save();
    }
  }

  /**
   * Course quiz options.
   */
  public function optionsDefinition() {
    $options = parent::optionsDefinition();

    $options['passing_grade'] = 75;

    return $options;
  }

  /**
   * Add an option only pertinent to quiz?
   */
  public function optionsForm(&$form, &$form_state) {
    parent::optionsForm($form, $form_state);
    $defaults = $this->getOptions();

    $form['passing_grade'] = array(
      '#title' => 'Passing grade',
      '#type' => 'textfield',
      '#default_value' => $defaults['passing_grade'],
    );

    return $form;
  }

  /**
   * Update the Quiz's passing grade.
   *
   * Let's do this as a courtesy.
   */
  public function optionsSubmit(&$form, &$form_state) {
    $passing_grade = $form_state['values']['passing_grade'];
    $quiz = node_load($this->getInstanceId());
    $quiz->pass_rate = $passing_grade;
    node_save($quiz);
    parent::optionsSubmit($form, $form_state);
  }

  public function getWarnings() {
    $warnings = parent::getWarnings();

    if ($this->node->nid) {
      if (!quiz_get_number_of_questions($this->node->vid)) {
        $warnings[] = t("The quiz %q in your course does not have any questions.", array('%q' => $this->getTitle()));
      }
    }

    return $warnings;
  }

}
