<?php

/**
 * Implements hook_views_default_views().
 */
function course_signup_views_default_views() {
  $views = array();

  // Exported view: course_signup_user_search_add
  $view = new view;
  $view->name = 'course_signup_user_search_add';
  $view->description = '';
  $view->tag = '';
  $view->base_table = 'users';
  $view->human_name = '';
  $view->core = 0;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['title'] = '%1: Search and Enroll';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer all signups';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'bulk';
  $handler->display->display_options['style_options']['columns'] = array(
    'name' => 'name',
    'realname' => 'realname',
    'mail' => 'mail',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'realname' => array(
      'align' => '',
      'separator' => '',
    ),
    'mail' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  $handler->display->display_options['style_options']['empty_table'] = 0;
  $handler->display->display_options['style_options']['operations'] = array(
    'user_block_ip_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'label' => '',
    ),
    'user_block_user_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'label' => '',
    ),
    'user_user_operations_block' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'label' => '',
    ),
    'views_bulk_operations_delete_user_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'label' => '',
    ),
    'system_message_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'label' => '',
    ),
    'course_add_enrollment_action' => array(
      'selected' => 1,
      'skip_confirmation' => 0,
      'label' => '',
    ),
    'course_signup_add_enrollment_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'label' => '',
    ),
    'views_bulk_operations_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'label' => '',
    ),
    'views_bulk_operations_script_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'label' => '',
    ),
    'views_bulk_operations_user_roles_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'label' => '',
    ),
    'views_bulk_operations_argument_selector_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'label' => '',
    ),
    'system_goto_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'label' => '',
    ),
    'course_remove_enrollment_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'label' => '',
    ),
    'system_send_email_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'label' => '',
    ),
    'user_user_operations_unblock' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'label' => '',
    ),
  );
  $handler->display->display_options['style_options']['execution_type'] = '1';
  $handler->display->display_options['style_options']['display_type'] = '0';
  $handler->display->display_options['style_options']['display_result'] = 1;
  $handler->display->display_options['style_options']['merge_single_action'] = 1;
  $handler->display->display_options['style_options']['hide_selector'] = 0;
  $handler->display->display_options['style_options']['preserve_selection'] = 1;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = 'Username';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name']['link_to_user'] = 0;
  $handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['realname']['id'] = 'realname';
  $handler->display->display_options['fields']['realname']['table'] = 'realname';
  $handler->display->display_options['fields']['realname']['field'] = 'realname';
  $handler->display->display_options['fields']['realname']['label'] = 'Full Name';
  $handler->display->display_options['fields']['realname']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['realname']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['realname']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['realname']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['realname']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['realname']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['realname']['alter']['html'] = 0;
  $handler->display->display_options['fields']['realname']['hide_empty'] = 0;
  $handler->display->display_options['fields']['realname']['empty_zero'] = 0;
  /* Field: User: E-mail */
  $handler->display->display_options['fields']['mail']['id'] = 'mail';
  $handler->display->display_options['fields']['mail']['table'] = 'users';
  $handler->display->display_options['fields']['mail']['field'] = 'mail';
  $handler->display->display_options['fields']['mail']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['mail']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['mail']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['mail']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['mail']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['mail']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['mail']['alter']['html'] = 0;
  $handler->display->display_options['fields']['mail']['hide_empty'] = 0;
  $handler->display->display_options['fields']['mail']['empty_zero'] = 0;
  $handler->display->display_options['fields']['mail']['link_to_user'] = '0';
  /* Argument: Global: Null */
  $handler->display->display_options['arguments']['null']['id'] = 'null';
  $handler->display->display_options['arguments']['null']['table'] = 'views';
  $handler->display->display_options['arguments']['null']['field'] = 'null';
  $handler->display->display_options['arguments']['null']['style_plugin'] = 'default_summary';
  $handler->display->display_options['arguments']['null']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['null']['validate_type'] = 'course';
  $handler->display->display_options['arguments']['null']['must_not_be'] = 0;
  /* Filter: User: Authentication name */
  $handler->display->display_options['filters']['authname']['id'] = 'authname';
  $handler->display->display_options['filters']['authname']['table'] = 'authmap';
  $handler->display->display_options['filters']['authname']['field'] = 'authname';
  $handler->display->display_options['filters']['authname']['operator'] = 'contains';
  $handler->display->display_options['filters']['authname']['exposed'] = TRUE;
  $handler->display->display_options['filters']['authname']['expose']['operator'] = 'authname_op';
  $handler->display->display_options['filters']['authname']['expose']['label'] = 'Username';
  $handler->display->display_options['filters']['authname']['expose']['identifier'] = 'authname';
  $handler->display->display_options['filters']['authname']['expose']['optional'] = 1;
  $handler->display->display_options['filters']['authname']['case'] = 0;
  /* Filter: Broken/missing handler */
  $handler->display->display_options['filters']['realname']['id'] = 'realname';
  $handler->display->display_options['filters']['realname']['table'] = 'realname';
  $handler->display->display_options['filters']['realname']['field'] = 'realname';
  $handler->display->display_options['filters']['realname']['operator'] = 'contains';
  $handler->display->display_options['filters']['realname']['exposed'] = TRUE;
  $handler->display->display_options['filters']['realname']['expose']['operator'] = 'realname_op';
  $handler->display->display_options['filters']['realname']['expose']['label'] = 'Full Name';
  $handler->display->display_options['filters']['realname']['expose']['identifier'] = 'realname';
  /* Filter: User: E-mail */
  $handler->display->display_options['filters']['mail']['id'] = 'mail';
  $handler->display->display_options['filters']['mail']['table'] = 'users';
  $handler->display->display_options['filters']['mail']['field'] = 'mail';
  $handler->display->display_options['filters']['mail']['operator'] = 'contains';
  $handler->display->display_options['filters']['mail']['exposed'] = TRUE;
  $handler->display->display_options['filters']['mail']['expose']['operator'] = 'mail_op';
  $handler->display->display_options['filters']['mail']['expose']['label'] = 'User: E-mail';
  $handler->display->display_options['filters']['mail']['expose']['identifier'] = 'mail';
  $handler->display->display_options['filters']['mail']['expose']['optional'] = 1;
  $handler->display->display_options['filters']['mail']['case'] = 0;
  /* Filter: User: Name */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'users';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['operator'] = 'not in';
  $handler->display->display_options['filters']['uid']['value'] = array(
    0 => 0,
  );
  $handler->display->display_options['filters']['uid']['expose']['operator'] = FALSE;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'bulk';
  $handler->display->display_options['style_options']['columns'] = array(
    'name' => 'name',
    'realname' => 'realname',
    'mail' => 'mail',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'realname' => array(
      'align' => '',
      'separator' => '',
    ),
    'mail' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  $handler->display->display_options['style_options']['empty_table'] = 0;
  $handler->display->display_options['style_options']['operations'] = array(
    'user_block_ip_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'label' => '',
    ),
    'user_block_user_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'label' => '',
    ),
    'user_user_operations_block' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'label' => '',
    ),
    'views_bulk_operations_delete_user_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'label' => '',
    ),
    'system_message_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'label' => '',
    ),
    'course_add_enrollment_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'label' => '',
    ),
    'course_signup_add_enrollment_action' => array(
      'selected' => 1,
      'skip_confirmation' => 0,
      'label' => '',
    ),
    'views_bulk_operations_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'label' => '',
    ),
    'views_bulk_operations_script_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'label' => '',
    ),
    'views_bulk_operations_user_roles_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'label' => '',
    ),
    'views_bulk_operations_argument_selector_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'label' => '',
    ),
    'system_goto_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'label' => '',
    ),
    'course_remove_enrollment_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'label' => '',
    ),
    'system_send_email_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'label' => '',
    ),
    'user_user_operations_unblock' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'label' => '',
    ),
  );
  $handler->display->display_options['style_options']['execution_type'] = '1';
  $handler->display->display_options['style_options']['display_type'] = '0';
  $handler->display->display_options['style_options']['display_result'] = 1;
  $handler->display->display_options['style_options']['merge_single_action'] = 1;
  $handler->display->display_options['style_options']['hide_selector'] = 0;
  $handler->display->display_options['style_options']['preserve_selection'] = 1;
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['path'] = 'node/%/signups/search-add';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Search and Enroll';
  $handler->display->display_options['menu']['weight'] = '0';

  $views[$view->name] = $view;

  // Exported view: course_signup_user_vbo_admin
  $view = new view;
  $view->name = 'course_signup_user_vbo_admin';
  $view->description = 'Per-node signup administration interface with checkboxes for bulk operations.';
  $view->tag = 'Signup';
  $view->base_table = 'signup_log';
  $view->human_name = '';
  $view->core = 0;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'bulk';
  $handler->display->display_options['style_options']['columns'] = array(
    'name' => 'name',
    'signup_time' => 'signup_time',
    'timestamp' => 'timestamp',
    'enrol_end' => 'enrol_end',
    'attended' => 'attended',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'signup_time' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'timestamp' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'enrol_end' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'attended' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  $handler->display->display_options['style_options']['empty_table'] = 0;
  $handler->display->display_options['style_options']['operations'] = array(
    'signup_cancel_action' => array(
      'selected' => 1,
      'skip_confirmation' => 0,
      'label' => '',
    ),
    'system_message_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'label' => '',
    ),
    'course_edit_enrollment_action' => array(
      'selected' => 1,
      'skip_confirmation' => 0,
      'label' => '',
    ),
    'views_bulk_operations_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'label' => '',
    ),
    'views_bulk_operations_script_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'label' => '',
    ),
    'signup_mark_attended_action' => array(
      'selected' => 1,
      'skip_confirmation' => 0,
      'label' => '',
    ),
    'signup_mark_not_attended_action' => array(
      'selected' => 1,
      'skip_confirmation' => 0,
      'label' => '',
    ),
    'views_bulk_operations_argument_selector_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'label' => '',
    ),
    'system_goto_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'label' => '',
    ),
    'system_send_email_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'label' => '',
    ),
  );
  $handler->display->display_options['style_options']['execution_type'] = '1';
  $handler->display->display_options['style_options']['display_type'] = '0';
  $handler->display->display_options['style_options']['display_result'] = 1;
  $handler->display->display_options['style_options']['merge_single_action'] = 1;
  $handler->display->display_options['style_options']['hide_selector'] = 0;
  $handler->display->display_options['style_options']['preserve_selection'] = 1;
  /* Relationship: Signup: Node */
  $handler->display->display_options['relationships']['nid']['id'] = 'nid';
  $handler->display->display_options['relationships']['nid']['table'] = 'signup_log';
  $handler->display->display_options['relationships']['nid']['field'] = 'nid';
  $handler->display->display_options['relationships']['nid']['required'] = 1;
  /* Relationship: Signup: User */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'signup_log';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['required'] = 1;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name']['link_to_user'] = 1;
  $handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
  /* Field: Signup: User: Signup Time */
  $handler->display->display_options['fields']['signup_time']['id'] = 'signup_time';
  $handler->display->display_options['fields']['signup_time']['table'] = 'signup_log';
  $handler->display->display_options['fields']['signup_time']['field'] = 'signup_time';
  $handler->display->display_options['fields']['signup_time']['label'] = 'Signup time';
  /* Field: Course enrolments: Start of enrolment */
  $handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['table'] = 'course_enrolment';
  $handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['relationship'] = 'nid';
  $handler->display->display_options['fields']['timestamp']['label'] = 'Enroll start';
  $handler->display->display_options['fields']['timestamp']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['external'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['timestamp']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['timestamp']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['html'] = 0;
  $handler->display->display_options['fields']['timestamp']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['timestamp']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['timestamp']['hide_empty'] = 0;
  $handler->display->display_options['fields']['timestamp']['empty_zero'] = 0;
  $handler->display->display_options['fields']['timestamp']['hide_alter_empty'] = 1;
  /* Field: Course enrolments: End of enrolment */
  $handler->display->display_options['fields']['enrol_end']['id'] = 'enrol_end';
  $handler->display->display_options['fields']['enrol_end']['table'] = 'course_enrolment';
  $handler->display->display_options['fields']['enrol_end']['field'] = 'enrol_end';
  $handler->display->display_options['fields']['enrol_end']['relationship'] = 'nid';
  $handler->display->display_options['fields']['enrol_end']['label'] = 'Enroll End';
  $handler->display->display_options['fields']['enrol_end']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['enrol_end']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['enrol_end']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['enrol_end']['alter']['external'] = 0;
  $handler->display->display_options['fields']['enrol_end']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['enrol_end']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['enrol_end']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['enrol_end']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['enrol_end']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['enrol_end']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['enrol_end']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['enrol_end']['alter']['html'] = 0;
  $handler->display->display_options['fields']['enrol_end']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['enrol_end']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['enrol_end']['hide_empty'] = 0;
  $handler->display->display_options['fields']['enrol_end']['empty_zero'] = 0;
  $handler->display->display_options['fields']['enrol_end']['hide_alter_empty'] = 1;
  /* Field: Signup: User: Attended */
  $handler->display->display_options['fields']['attended']['id'] = 'attended';
  $handler->display->display_options['fields']['attended']['table'] = 'signup_log';
  $handler->display->display_options['fields']['attended']['field'] = 'attended';
  $handler->display->display_options['fields']['attended']['label'] = 'Attendance';
  /* Argument: Node: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['relationship'] = 'nid';
  $handler->display->display_options['arguments']['nid']['style_plugin'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nid']['validate_type'] = 'signup_status';
  $handler->display->display_options['arguments']['nid']['break_phrase'] = 0;
  $handler->display->display_options['arguments']['nid']['not'] = 0;

  $views[$view->name] = $view;

  return $views;
}
