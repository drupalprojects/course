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
}
