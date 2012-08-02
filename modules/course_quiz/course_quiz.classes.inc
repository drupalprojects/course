<?php

class CourseObjectQuiz extends CourseObjectNode {

  /**
   * Create the quiz node and set it as this object's instance.
   */
  function create() {
    $quiz = new stdClass;
    $quiz->auto_created = TRUE;
    $quiz->type = 'quiz';
    $quiz->title = $this->getTitle();
    $quiz->uid = $this->user->uid;
    $quiz->quiz_always = TRUE;
    $quiz->quiz_open = $quiz->quiz_close = array(
      'month' => date('m'),
      'day' => date('d'),
      'year' => date('Y'),
    );
    $quiz = (object) array_merge(_quiz_get_node_defaults(), (array) $quiz);
    node_save($quiz);
    $this->setNode($quiz);
  }

  /**
   * The take URL of the quiz is /take.
   */
  function getTakeUrl() {
    return url("node/{$this->node->nid}/take");
  }

  /**
   * Marks a user's fulfillment record for this object complete if the user
   * passed the quiz.
   */
  function grade($user, $rid) {
    $nid = (int) $this->getInstanceId();
    $fulfillment = $this->getFulfillment();

    $result_ids = (array) $fulfillment->getOption('quiz_result_ids');
    $result_ids[] = $rid;
    $fulfillment->setOption('quiz_result_ids', $result_ids);

    $result = reset(quiz_get_score_data(array($nid), $user->uid));
    if ($result && ($result->percent_score >= $this->getOption('passing_grade'))) {
      $fulfillment->setGrade($result->percent_score)->setComplete()->save();
    }
    else {
      $fulfillment->setGrade($result->percent_score)->save();
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
  }

  /**
   * Let the user know if they have a Quiz without questions.
   */
  public function getWarnings() {
    $warnings = parent::getWarnings();

    if (isset($this->node->nid)) {
      if (!quiz_get_number_of_questions($this->node->vid)) {
        $warnings[] = t("The quiz %q in your course does not have any questions.", array('%q' => $this->getTitle()));
      }
    }

    return $warnings;
  }

  public function getReports() {
    return array(
      'results' => array(
        'title' => 'Results',
      ),
    );
  }

  public function getReport($key) {
    module_load_include('inc', 'quiz', 'quiz.admin');
    switch ($key) {
      case 'results':
        return array(
          'title' => t('Quiz results'),
          'content' => drupal_get_form('quiz_results_manage_results_form', $this->node),
        );
    }
  }

  /**
   * Remove all quiz attempts associated with this fulfillment.
   */
  public function unenroll() {
    parent::unenroll();
    $fulfillment = $this->getFulfillment();
    quiz_delete_results((array) $fulfillment->getOption('quiz_result_ids'));
  }

  function getNodeTypes() {
    return array('quiz');
  }

}
