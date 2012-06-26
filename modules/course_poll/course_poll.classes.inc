<?php

/**
 * Parent class for poll tracking.
 */
class CourseObjectPoll extends CourseObjectNode {

  function getTakeType() {
    return 'content';
  }

  /**
   * Take or view the results of a poll. Wrapper for poll_view().
   */
  function take() {
    poll_view($this->node);
    return drupal_render($this->node->content['body']);
  }

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

  public function getReports() {
    return array(
      'results' => array(
        'title' => 'Results',
      ),
      'all' => array(
        'title' => 'List votes',
      ),
    );
  }

  public function getReport($key) {
    module_load_include('inc', 'poll', 'poll.pages');
    switch ($key) {
      case 'results':
        return array(
          'title' => t('Poll results'),
          'content' => poll_view_results($this->node, NULL, NULL, NULL),
        );
      case 'all':
        return array(
          'title' => 'All votes',
          'content' => poll_votes($this->node),
        );
    }
  }

}
