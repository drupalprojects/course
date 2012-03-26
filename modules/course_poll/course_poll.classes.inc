<?php

/**
 * Parent class for webform course tracking. 
 */
class CourseObjectPoll extends CourseObjectNode {

  public function create() {
    $poll = new stdClass;
    $poll->choice[0]['chtext'] = 'Yes';
    $poll->choice[1]['chtext'] = 'No';
    $poll->type = 'poll';
    $poll->title = $this->getTitle();
    $poll->uid = $this->node->uid;
    $poll->active = 1;
    node_save($poll);
    $this->setInstanceId($poll->nid);
  }

}
