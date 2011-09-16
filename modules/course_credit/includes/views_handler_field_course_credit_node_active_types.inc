<?php

/**
 * Field handler to provide a list of credit titles.
 */
class views_handler_field_course_credit_node_active_types extends views_handler_field_prerender_list {
  /**
   * Working with node table, adding active credit types to it.
   */
  function construct() {
    parent::construct();
    $this->additional_fields['nid'] = array('table' => 'node', 'field' => 'nid');
  }

  /**
   * Add fields, alias.
   */
  function query() {
    $this->add_additional_fields();
    $this->field_alias = $this->aliases['nid'];
  }

  /**
   * Get the active credit types on each of the nodes in this view.
   */
  function pre_render($values) {
    foreach ($values as $value) {
      $nids[] = $value->nid;
    }

    $placeholders = db_placeholders($nids);
    $sql = "SELECT cc.nid, cc.ctid, cct.title FROM {course_credit} cc
      LEFT JOIN {course_credit_type} cct USING (ctid)
      WHERE cc.active AND nid IN ($placeholders)";
    $result = db_query($sql, $nids);
    while ($row = db_fetch_array($result)) {
      $this->items[$row['nid']][$row['ctid']] = $row;
    }
  }

  /**
   * Render each credit title.
   */
  function render_item($count, $item) {
    return $item['title'];
  }
}
