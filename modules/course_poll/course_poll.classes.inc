<?php

/**
 * Parent class for poll tracking.
 */
class CourseObjectPoll extends CourseObjectNode {

  public function create() {
    global $user;
    $poll = new stdClass;
    $poll->choice[0]['chtext'] = 'Yes';
    $poll->choice[1]['chtext'] = 'No';
    $poll->type = 'poll';
    $poll->title = $this->getTitle();
    $poll->uid = $user->uid;
    $poll->active = 1;
    node_save($poll);
    $this->setInstanceId($poll->nid);
  }
}
