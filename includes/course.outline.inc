<?php

/**
 * @file course_outline.inc
 *
 * Functions to show and edit graphical course outline.
 */

/**
 * Theme all requirements as a table.
 */
function course_outline_overview_form(&$form, &$form_state = NULL, $form_id = NULL) {
  // Load the modal library and add the modal javascript.
  ctools_include('modal');
  ctools_include('dependent');
  ctools_modal_add_js();

  // Wrapper for objects and more button.
  $form['course_outline']['#cache'] = TRUE;
  $form['course_outline']['#tree'] = TRUE;
  $form['course_outline']['#type'] = 'fieldset';
  $form['course_outline']['requirements']['#prefix'] = '<div class="clear-block" id="course-objects-wrapper">';
  $form['course_outline']['requirements']['#suffix'] = '</div>';

  // Shortcut.
  $cform = &$form['course_outline'];
  $node = $form['#node'];
  if (!$node) {
    $node = node_load(arg(1));
  }
  $requirements = $node->course_outline['requirements'];
  if (is_array($form_state['values']['course_outline']['requirements'])) {
    $requirements = $form_state['values']['course_outline']['requirements'];
  }

  $cform['#title'] = 'Course objects';
  $cform['requirements']['#theme'] = 'course_outline_overview_form';

  $course_objects = course_available_objects();
  $options = array(
    '' => '- select new -',
  );
  foreach ($course_objects as $module => $objects) {
    if (count($objects) == 1) {
      $pair = each($objects);
      $key = $pair[0];
      $object = $pair[1];
      $options[$object['module'] . '-' . $key] = $object['title'];
    }
    else {
      foreach ($objects as $key => $object) {
        $module_string = ucwords(str_replace('_', ' ', $module));
        $options[$object['module'] . '-' . $key] = $module_string . ': ' . $object['title'];
      }
    }
  }

  // Wrapper for just the objects.
  $cform['requirements']['#tree'] = TRUE;
  if (count($requirements)) {
    foreach ($requirements as $requirement) {
      $requirement = (array) $requirement;
      if ($requirement['hidden']) {
        // Do not print out hidden requirements yet, such as attendance/payment.
        continue;
      }
      $uniqid = uniqid();
      $rform = _course_outline_requirement_form($requirement, $uniqid);
      if (empty($requirement['snid'])) {
        // User hasnt selected an object type yet, let them select.
        $rform['requirement_component'] = array(
          '#type' => 'select',
          '#options' => $options,
          '#default_value' => $requirement['requirement_component'],
        );
      }
      else {
        // Don't allow user to change type of object.
        $rform['requirement_component'] = array(
          '#type' => 'hidden',
          '#value' => $requirement['requirement_component'],
        );
        $rform['requirement_component_show'] = array(
          '#type' => 'markup',
          '#value' => ucwords(str_replace('_', ' ', $requirement['requirement_type'])) . '<br/>' . $course_objects[$requirement['requirement_type']][$requirement['requirement_component']]['title'],
        );
      }
      $cform['requirements'][$uniqid] = $rform;
    }
  }

  $uniqid = uniqid();
  $new = _course_outline_requirement_form(array(), $uniqid);
  $new['requirement_component'] = array(
    '#title' => 'Component',
    '#type' => 'select',
    '#options' => $options,
    '#suffix' => "<div id=\"edit-course-outline-add-scorm-file-div\">
    </div>",
  );
  $cform['requirements'][$uniqid] = $new;

  $cform['course_object_more'] = array(
    '#type' => 'submit',
    '#value' => 'Add another',
    '#submit' => array('course_outline_overview_add'),
    '#ahah' => array(
      'path' => 'course-outline/js',
      'wrapper' => 'course-objects-wrapper',
    ),
  );

  $cform['requirements']['#element_validate'] = array('_course_outline_overview_validate_requirements');
  //return for form on tab
  return $form['course_outline'];
}

/**
 * Define a form for 1 requirement.
 *
 * To be re-used in listing requirements and creating new ones.
 */
function _course_outline_requirement_form($requirement = array(), $uniqid = NULL) {
  $courseObject = course_get_course_object($requirement);
  $rform['#tree'] = TRUE;
  $rform['snid'] = array(
    '#type' => 'value',
    '#default_value' => $requirement['snid'],
  );
  $rform['instance'] = array(
    '#type' => 'value',
    '#default_value' => $requirement['instance'],
  );
  $rform['requirement_type'] = array(
    '#type' => 'value',
    '#default_value' => $requirement['requirement_type'],
  );
  $rform['requirement_component'] = array(
    '#type' => 'value',
    '#default_value' => $requirement['requirement_component'],
  );
  $rform['nid'] = array(
    '#type' => 'value',
    '#default_value' => $requirement['nid'],
  );
  $rform['title'] = array(
    '#title' => 'Title',
    '#type' => 'textfield',
    '#size' => 16,
    '#default_value' => $requirement['title'],
    '#description' => $requirement['add'],
  );
  $rform['options']['required'] = array(
    '#title' => 'Required',
    '#type' => 'checkbox',
    '#default_value' => isset($requirement['required']) ? $requirement['required'] : 1,
  );
  $rform['options']['enabled'] = array(
    '#title' => 'Enabled',
    '#type' => 'checkbox',
    '#default_value' => isset($requirement['enabled']) ? $requirement['enabled'] : 1,
  );
  $rform['options']['delete'] = array(
    '#title' => 'Delete',
    '#type' => 'checkbox',
  );
  if ($requirement['instance']) {
    $rform['options']['delete_instance'] = array(
      '#title' => 'Delete instance too',
      '#type' => 'checkbox',
      '#process' => array('ctools_dependent_process'),
      '#dependency' => array('edit-course-outline-requirements-' . $uniqid . '-options-delete' => array(1)),
    );
  }
  $rform['operations'] = array(
    '#type' => 'markup',
  );
  $rform['operations']['#value'] = implode(' ', array(
    l('Edit', $courseObject->getEditUrl(), array('external' => TRUE, 'query' => drupal_get_destination())),
    l('Settings', "node/" . arg(1) . '/course-object/' . $requirement['snid'] . '/nojs/edit', array('attributes' => array('class' => 'ctools-use-modal'))),
    ));
  $rform['weight'] = array(
    '#type' => 'weight',
    '#default_value' => $requirement['weight'],
    '#attributes' => array(
      'class' => 'course-object-weight',
    ),
  );

  return $rform;
}

/**
 * Non-ahah version for when the node is available.
 */
function course_outline_overview_add($form, $form_state) {
  // Non-ahah version.
}

/**
 * Rebuild the form with new fields.
 */
function course_outline_overview_js_more() {
  // The form is generated in an include file which we need to include manually.
  include_once 'modules/node/node.pages.inc';

  // We're starting in step #3, preparing for #4.
  $form_state = array('storage' => NULL, 'submitted' => FALSE);
  $form_build_id = $_POST['form_build_id'];
  // Step #4.
  $form = form_get_cache($form_build_id, $form_state);

  // Preparing for #5.
  $args = $form['#parameters'];
  $form_id = array_shift($args);
  $form_state['post'] = $form['#post'] = $_POST;
  $form['#programmed'] = $form['#redirect'] = FALSE;

  // Step #5.
  drupal_process_form($form_id, $form, $form_state);

  // Step #6 and #7 and #8.
  $form = drupal_rebuild_form($form_id, $form_state, $args, $form_build_id);

  $object_form = $form['course_outline']['requirements'];

  $output = theme('status_messages') . drupal_render($object_form);

  // Final rendering callback.
  drupal_json(array('status' => TRUE, 'data' => $output));
}

function theme_course_outline_overview_form($form) {
  if ($form['requirements']) {
    $requirements = & $form['requirements'];
  }
  else {
    $requirements = & $form;
  }
  drupal_add_tabledrag('course-objects', 'order', 'sibling', 'course-object-weight');
  $node = $form['#node'];

  $headers = array('Title', 'Type', 'Options', 'Operations', 'Weight');

  $rows = array();

  foreach (element_children($requirements) as $key) {
    unset($requirements[$key]['title']['#title']);
    unset($requirements[$key]['requirement_component']['#title']);
    $rows[$key]['data'][] = drupal_render($requirements[$key]['title']);
    $rows[$key]['data'][] = drupal_render($requirements[$key]['requirement_component']) . drupal_render($requirements[$key]['requirement_component_show']);
    $rows[$key]['data'][] = drupal_render($requirements[$key]['options']);
    $rows[$key]['data'][] = drupal_render($requirements[$key]['operations']);

    // Add draggable settings to row.
    $rows[$key]['class'] = 'draggable';
    $rows[$key]['data'][] = drupal_render($requirements[$key]['weight']);

    // Mark everything else as printed.
    drupal_render($requirements[$key]);
  }
  $table = theme('table', $headers, $rows, array('id' => 'course-objects'));
  unset($form['#theme']);
  unset($form['requirements']['#theme']);
  $table .= drupal_render($form);
  return $table;
}

/**
 * Validation callback.
 */
function _course_outline_overview_validate_requirements(&$form, &$form_state) {
  foreach ($form_state['values']['course_outline']['requirements'] as $key => $item) {
    if ($item['requirement_component'] && !$item['title']) {
      form_error($form[$key]['title'], t('Title is required.'));
    }
  }
}

/**
 * Submit handler.
 */
function course_outline_overview_save($node, $op) {
  if (isset($node->course_outline)) {
    foreach ($node->course_outline['requirements'] as $key => $record) {
      $record = array_merge($record, $record['options']);
      $record = (object) $record;
      $record->nid = $node->nid;
      if ($record->delete && $record->snid) {
        $courseObject = course_get_course_object($record);
        if ($record->delete_instance) {
          // Delete the instance (node, whatever).
          $courseObject->delete();
        }
        course_outline_delete_requirement($record);
      }
      else if (!$record->delete && !empty($record->requirement_component)) {
        // Must have a component.
        course_outline_save_requirement($record, $node);
      }
    }
  }
}

/**
 * Generate HTML of the course outline.
 *
 * @param object $node
 *
 * @return course outline list.
 */
function course_get_outline($node) {
  global $user;

  // Array of the available course objects (to lookup modules).
  $object_map = course_available_objects();

  // Create an array of the list of course objects.
  $sql = "SELECT * FROM {course_outline} cr WHERE nid = %d AND enabled AND !hidden ORDER BY weight ASC";
  $result = db_query($sql, $node->nid);
  $course_reqs = array();
  $course_objects = array();
  while ($row = db_fetch_object($result)) {
    $course_objects[] = course_get_course_object($row, $user);
  }

  // Iterate over objects.
  foreach ($course_objects as $key => $courseObject) {
    $step = array();

    // Find out what this step's status is.
    // Check the previous step.
    $prev_req = $course_objects[($key - 1)];
    if ($courseObject->getFulfillment()->isComplete()) {
      // Step is complete. User can access.
      $step['link_style'] = 'completed';
      $step['status_text'] = 'Complete';
      $step['link_to_step'] = 'node/' . $courseObject->getCourseNid() . '/course-object/' . $courseObject->getId();
    }
    else if (!$courseObject->getFulfillment()->isComplete() && (!$prev_req || $prev_req->getFulfillment()->isComplete())) {
      // Check if this is incomplete, but next. User can access.
      $step['link_style'] = 'in-progress';
      $step['status_text'] = 'Begin';
      $step['link_to_step'] = 'node/' . $courseObject->getCourseNid() . '/course-object/' . $courseObject->getId();
    }
    else {
      // User cannot access this step yet.
      $step['link_style'] = 'not-started';
      $step['link_to_step'] = '';
    }

    // arrowitem = "<span class='arrow-step'></span>";
    $class_more = '';
    drupal_alter('course_outline_step', $step, $node, $user);
    if (!$step['link_to_step']) {
      $data = $courseObject->getTitle();
    }
    else {
      $data = l("{$courseObject->getTitle()}<br/>", $step['link_to_step'], array('html' => TRUE)) . '<span class="course-outline-status">' . $step['status_text'] . '</span>';
    }
    $workflow[] = array(
      'data' => $data,
      'id' => $step['id_text'],
      'class' => "{$step['link_style']} $class_more $arrowclass",
    );
  }

  $last_step_style = $link_style;
  $link_to_step = url("node/{$node->nid}/course-complete");

  // Allow other modules to alter the workflow if needed.
  drupal_alter('course_outline', $workflow);

  $output .= theme_item_list($workflow);

  return $output;
}

/**
 * Render a landing page for course completion.
 *
 * Modules can use hook_course_outline_completion_links_alter() to add links
 * to this page, such as post-course actions.
 *
 * @param $course_node A course node.
 *
 * @return string HTML for the landing page.
 */
function course_outline_show_complete_links($course_node) {
  global $user;
  $account = $user;
  $report = course_report_load($course_node, $account);
  if ($report->complete) {
    $txt_out .= '<p>' . t('Thank you for participating in this activity.') . '</p>';
    $links = array(
      'course' => array('Return to course', "node/$course_node->nid", 'Return to the course to view course details and material.'),
    );
    drupal_alter('course_outline_completion_links', $links, $course_node, $account);
    foreach ($links as $link) {
      $txt_out .= theme_form_element(array('#description' => $link[2]), l($link[0], $link[1]));
    }
  }
  else {
    $requirements_outstanding = course_outline_check_remaining_requirements($course_node, $account);
    $items = array();
    foreach ($requirements_outstanding as $req) {
      $status_css = $req['complete'] ? 'complete' : 'incomplete';
      $status_img = $req['complete'] ? 'ok' : ($req['required'] ? 'error' : 'warning');
      $status_optional = ($status_img == 'warning') ? ' (optional)' : '';
      $grade = $req['graded'] ? ' - Your grade: ' . $req['grade_result'] . '%, Pass grade: ' . $req['passing_grade'] . '%' : '';
      $items[] = array(
        array(
          'data' => theme_image("misc/watchdog-{$status_img}.png", $status_css),
          'width' => 20,
        ),
        $req['title'] . $status_optional . $grade,
      );
    }
    $reqs_txt = theme_table(NULL, $items);

    $txt_out .= '<p>' . t('You must complete the remaining requirements to proceed.') . '</p>';
    $txt_out .= $reqs_txt . '<div class="homelink"><p>' . l($course_out_return_array['course_outline_link_text'], $course_out_return_array['course_outline_link']) . '</p></div>';

    $links = array(
      'course' => array('Return to course', "node/$course_node->nid/takecourse", 'Return to the course to view course details and material.'),
    );
    drupal_alter('course_outline_incomplete_links', $links, $course_node, $account);
    foreach ($links as $link) {
      $txt_out .= theme_form_element(array('#description' => $link[2]), l($link[0], $link[1]));
    }
  }

  return $txt_out;
}

/**
 * Get a list of requirements and the user's fullfillment.
 */
function course_outline_check_remaining_requirements($course_node, $account) {
  $requirements_outstanding = array();

  $sql = "select crf.*, cr.* from {course_outline} cr
  left join {course_outline_fulfillment} crf on (cr.snid = crf.snid and uid = %d)
  where nid = %d and enabled
  order by weight asc";

  $results = db_query($sql, $account->uid, $course_node->nid);
  while ($row = db_fetch_object($results)) {
    $requirements_outstanding[] = $row;
  }

  return $requirements_outstanding;
}

/**
 * Delete a fulfillment.
 */
function course_outline_delete_fulfillment($mixed) {
  $sql = "delete from {course_outline_fulfillment} where snid = %d and uid = %d";
  db_query($sql, $mixed->snid, $mixed->uid);
  return db_affected_rows() > 0;
}
