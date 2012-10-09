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
    poll_view($this->node, 'full');
    return drupal_render($this->node->content);
  }

  public function create() {
    $poll = new stdClass;
    $poll->choice = array(
      array(
        'chtext' => 'Yes',
        'chvotes' => 0,
        'weight' => 0,
      ),
      array(
        'chtext' => 'No',
        'chvotes' => 0,
        'weight' => 0,
      ),
    );
    $poll->type = 'poll';
    $poll->title = $this->getTitle();
    $poll->uid = $this->user->uid;
    $poll->active = 1;
    $poll->runtime = 0;
    node_save($poll);
    $this->setNode($poll);
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
          'content' => drupal_render(poll_votes($this->node)),
        );
    }
  }

  /**
   * Remove poll votes for this user.
   */
  function unEnroll() {
    db_delete('poll_vote')
      ->condition('nid', $this->node->nid)
      ->condition('uid', $this->getCourse()->getUser()->uid)
      ->execute();
    db_update('poll_choice')
      ->expression('chvotes', 'chvotes - 1')
      ->condition('nid', $this->node->nid)
      ->condition('weight', $this->node->vote)
      ->execute();
  }

  function getNodeTypes() {
    return array('poll');
  }

  function getCloneAbility() {
    return TRUE;
  }

}
