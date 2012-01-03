<?php

class CourseObjectQuiz extends CourseObjectNode {
  function create($object) {
    global $user;
    switch ($object->requirement_component) {
      case 'quiz':
        if ($object->instance) {
          $quiz = node_load($object->instance);
        }
        else {
          $quiz = new stdClass;
        }
        $quiz->auto_created = TRUE;
        $quiz->type = 'quiz';
        $quiz->title = $object->title;
        $quiz->uid = $user->uid;
        $quiz->quiz_always = TRUE;
        $quiz->quiz_open = $quiz->quiz_close = array(
          'month' => date('m'),
          'day' => date('d'),
          'year' => date('Y'),
        );
        $quiz = (object) array_merge(_quiz_get_node_defaults(), (array) $quiz);
        node_save($quiz);
        return $quiz->nid;
    }
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
