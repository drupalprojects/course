<?php

class CourseObjectQuiz extends CourseObjectNode {
  function getTakeUrl() {
    return url("node/{$this->node->nid}/take");
  }

  /**
   * Marks a user's fulfillment record for this object complete if the user
   * passed the quiz.
   */
  function grade($user) {
    $nid = (int) $this->getInstanceId();

    $results = quiz_get_score_data(array($nid), $user->uid);
    if ($results[$nid]->percent_score >= $results[$nid]->percent_pass) {
      $this->getFulfillment()->setGrade($results[$nid]->percent_score)->setComplete()->save();
    }
    else {
      $this->getFulfillment()->setGrade($results[$nid]->percent_score)->save();
    }
  }
}
