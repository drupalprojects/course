<?php

/**
 * Relate awarded credit to credit instance.
 *
 * Join awarded.nid to instance.nid, awarded.ctid to instance.ctid
 */
class course_credit_awarded_credit_instance_vhr extends views_handler_relationship {
  /**
   * Called to implement a relationship in a query.
   */
  function query() {
    // Figure out what base table this relationship brings to the party.
    // left table = the base table we already have
    $join = new views_join_course_credit();
    $join->definition = array(
      'left_table' => !empty($this->relationship) ? $this->relationship : 'course_credit_awarded',
      'left_field' => 'ctid',
      'table' => 'course_credit',
      'field' => 'ctid',
      'extra' => array(
        array(
          'field' => 'nid', // course_credit.nid
          'numeric' => TRUE,
          'value' => (!empty($this->relationship) ? $this->relationship : 'course_credit_awarded') . '.nid',
        ),
      ),
    );

    $join->construct();
    $this->ensure_my_table();

    $alias = $join->definition['table'] .'_'. $join->definition['left_table'];
    $this->alias = $this->query->add_relationship($alias, $join, $this->definition['base'], $this->relationship);
  }
}