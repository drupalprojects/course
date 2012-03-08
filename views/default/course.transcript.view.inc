<?php

$view = new view;
$view->name = 'transcript';
$view->description = '';
$view->tag = '';
$view->base_table = 'course_report';
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
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = '50';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['pager']['options']['id'] = '0';
$handler->display->display_options['pager']['options']['expose']['items_per_page_options_all'] = 0;
$handler->display->display_options['style_plugin'] = 'table';
/* Relationship: Course report: Course Node */
$handler->display->display_options['relationships']['nid']['id'] = 'nid';
$handler->display->display_options['relationships']['nid']['table'] = 'course_report';
$handler->display->display_options['relationships']['nid']['field'] = 'nid';
$handler->display->display_options['relationships']['nid']['required'] = 1;
/* Relationship: Course report: Course report user */
$handler->display->display_options['relationships']['uid']['id'] = 'uid';
$handler->display->display_options['relationships']['uid']['table'] = 'course_report';
$handler->display->display_options['relationships']['uid']['field'] = 'uid';
/* Relationship: Course credit awarded: Credit type */
$handler->display->display_options['relationships']['ctid_global']['id'] = 'ctid_global';
$handler->display->display_options['relationships']['ctid_global']['table'] = 'course_credit_awarded';
$handler->display->display_options['relationships']['ctid_global']['field'] = 'ctid_global';
$handler->display->display_options['relationships']['ctid_global']['relationship'] = 'credit_awarded';
$handler->display->display_options['relationships']['ctid_global']['required'] = 0;
/* Relationship: Course report: Course enrollment */
$handler->display->display_options['relationships']['course_enrollment']['id'] = 'course_enrollment';
$handler->display->display_options['relationships']['course_enrollment']['table'] = 'course_report';
$handler->display->display_options['relationships']['course_enrollment']['field'] = 'course_enrollment';
/* Relationship: Course report: Credit awarded */
$handler->display->display_options['relationships']['credit_awarded']['id'] = 'credit_awarded';
$handler->display->display_options['relationships']['credit_awarded']['table'] = 'course_report';
$handler->display->display_options['relationships']['credit_awarded']['field'] = 'credit_awarded';
/* Field: Node: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['relationship'] = 'nid';
$handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['title']['alter']['trim'] = 0;
$handler->display->display_options['fields']['title']['alter']['html'] = 0;
$handler->display->display_options['fields']['title']['hide_empty'] = 0;
$handler->display->display_options['fields']['title']['empty_zero'] = 0;
$handler->display->display_options['fields']['title']['link_to_node'] = 1;
/* Field: Course report: Credits */
$handler->display->display_options['fields']['cecredits']['id'] = 'cecredits';
$handler->display->display_options['fields']['cecredits']['table'] = 'course_report';
$handler->display->display_options['fields']['cecredits']['field'] = 'cecredits';
$handler->display->display_options['fields']['cecredits']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['cecredits']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['cecredits']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['cecredits']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['cecredits']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['cecredits']['alter']['trim'] = 0;
$handler->display->display_options['fields']['cecredits']['alter']['html'] = 0;
$handler->display->display_options['fields']['cecredits']['hide_empty'] = 0;
$handler->display->display_options['fields']['cecredits']['empty_zero'] = 0;
/* Field: Course report: Max credits */
$handler->display->display_options['fields']['cecreditsmax']['id'] = 'cecreditsmax';
$handler->display->display_options['fields']['cecreditsmax']['table'] = 'course_report';
$handler->display->display_options['fields']['cecreditsmax']['field'] = 'cecreditsmax';
$handler->display->display_options['fields']['cecreditsmax']['label'] = 'Max Credits';
$handler->display->display_options['fields']['cecreditsmax']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['cecreditsmax']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['cecreditsmax']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['cecreditsmax']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['cecreditsmax']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['cecreditsmax']['alter']['trim'] = 0;
$handler->display->display_options['fields']['cecreditsmax']['alter']['html'] = 0;
$handler->display->display_options['fields']['cecreditsmax']['hide_empty'] = 0;
$handler->display->display_options['fields']['cecreditsmax']['empty_zero'] = 0;
/* Field: Course report: Credits claimed */
$handler->display->display_options['fields']['credits_claimed']['id'] = 'credits_claimed';
$handler->display->display_options['fields']['credits_claimed']['table'] = 'course_report';
$handler->display->display_options['fields']['credits_claimed']['field'] = 'credits_claimed';
$handler->display->display_options['fields']['credits_claimed']['label'] = 'Claimed Credits';
$handler->display->display_options['fields']['credits_claimed']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['credits_claimed']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['credits_claimed']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['credits_claimed']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['credits_claimed']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['credits_claimed']['alter']['trim'] = 0;
$handler->display->display_options['fields']['credits_claimed']['alter']['html'] = 0;
$handler->display->display_options['fields']['credits_claimed']['hide_empty'] = 0;
$handler->display->display_options['fields']['credits_claimed']['empty_zero'] = 0;
/* Field: Course report: Course status */
$handler->display->display_options['fields']['section_name']['id'] = 'section_name';
$handler->display->display_options['fields']['section_name']['table'] = 'course_report';
$handler->display->display_options['fields']['section_name']['field'] = 'section_name';
$handler->display->display_options['fields']['section_name']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['section_name']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['section_name']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['section_name']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['section_name']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['section_name']['alter']['trim'] = 0;
$handler->display->display_options['fields']['section_name']['alter']['html'] = 0;
$handler->display->display_options['fields']['section_name']['hide_empty'] = 0;
$handler->display->display_options['fields']['section_name']['empty_zero'] = 0;
/* Field: Course enrolments: Start of enrolment */
$handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
$handler->display->display_options['fields']['timestamp']['table'] = 'course_enrolment';
$handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
$handler->display->display_options['fields']['timestamp']['label'] = 'Enrolled date';
$handler->display->display_options['fields']['timestamp']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['timestamp']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['timestamp']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['trim'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['html'] = 0;
$handler->display->display_options['fields']['timestamp']['hide_empty'] = 0;
$handler->display->display_options['fields']['timestamp']['empty_zero'] = 0;
/* Argument: User: Uid */
$handler->display->display_options['arguments']['uid']['id'] = 'uid';
$handler->display->display_options['arguments']['uid']['table'] = 'users';
$handler->display->display_options['arguments']['uid']['field'] = 'uid';
$handler->display->display_options['arguments']['uid']['relationship'] = 'uid';
$handler->display->display_options['arguments']['uid']['default_action'] = 'default';
$handler->display->display_options['arguments']['uid']['style_plugin'] = 'default_summary';
$handler->display->display_options['arguments']['uid']['default_argument_type'] = 'user';
$handler->display->display_options['arguments']['uid']['default_argument_options']['user'] = FALSE;
$handler->display->display_options['arguments']['uid']['validate_type'] = 'user';
$handler->display->display_options['arguments']['uid']['validate_options']['restrict_roles'] = 0;
$handler->display->display_options['arguments']['uid']['validate_options']['roles'] = array(
  2 => 0,
  3 => 0,
  4 => 0,
);
$handler->display->display_options['arguments']['uid']['break_phrase'] = 0;
$handler->display->display_options['arguments']['uid']['not'] = 0;
/* Filter: Course report: Complete */
$handler->display->display_options['filters']['complete']['id'] = 'complete';
$handler->display->display_options['filters']['complete']['table'] = 'course_report';
$handler->display->display_options['filters']['complete']['field'] = 'complete';
/* Filter: Course: Transcript */
$handler->display->display_options['filters']['transcript']['id'] = 'transcript';
$handler->display->display_options['filters']['transcript']['table'] = 'course_node';
$handler->display->display_options['filters']['transcript']['field'] = 'transcript';
$handler->display->display_options['filters']['transcript']['relationship'] = 'nid';
$handler->display->display_options['filters']['transcript']['value'] = '1';
$handler->display->display_options['filters']['transcript']['expose']['operator'] = FALSE;

/* Display: Pending */
$handler = $view->new_display('page', 'Pending', 'page_1');
$handler->display->display_options['defaults']['title'] = FALSE;
$handler->display->display_options['title'] = 'Pending Activities';
$handler->display->display_options['defaults']['empty'] = FALSE;
/* Empty text: Global: Text area */
$handler->display->display_options['empty']['text']['id'] = 'area';
$handler->display->display_options['empty']['text']['table'] = 'views';
$handler->display->display_options['empty']['text']['field'] = 'area';
$handler->display->display_options['empty']['text']['label'] = '';
$handler->display->display_options['empty']['text']['empty'] = FALSE;
$handler->display->display_options['empty']['text']['content'] = 'You don\'t have any pending activities.';
$handler->display->display_options['empty']['text']['format'] = '2';
$handler->display->display_options['defaults']['relationships'] = FALSE;
/* Relationship: Course report: Course Node */
$handler->display->display_options['relationships']['nid']['id'] = 'nid';
$handler->display->display_options['relationships']['nid']['table'] = 'course_report';
$handler->display->display_options['relationships']['nid']['field'] = 'nid';
$handler->display->display_options['relationships']['nid']['required'] = 1;
/* Relationship: Course report: Course report user */
$handler->display->display_options['relationships']['uid']['id'] = 'uid';
$handler->display->display_options['relationships']['uid']['table'] = 'course_report';
$handler->display->display_options['relationships']['uid']['field'] = 'uid';
/* Relationship: Course report: Course enrollment */
$handler->display->display_options['relationships']['course_enrollment']['id'] = 'course_enrollment';
$handler->display->display_options['relationships']['course_enrollment']['table'] = 'course_report';
$handler->display->display_options['relationships']['course_enrollment']['field'] = 'course_enrollment';
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Node: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['relationship'] = 'nid';
$handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['title']['alter']['trim'] = 0;
$handler->display->display_options['fields']['title']['alter']['html'] = 0;
$handler->display->display_options['fields']['title']['hide_empty'] = 0;
$handler->display->display_options['fields']['title']['empty_zero'] = 0;
$handler->display->display_options['fields']['title']['link_to_node'] = 1;
/* Field: Course report: Course status */
$handler->display->display_options['fields']['section_name']['id'] = 'section_name';
$handler->display->display_options['fields']['section_name']['table'] = 'course_report';
$handler->display->display_options['fields']['section_name']['field'] = 'section_name';
$handler->display->display_options['fields']['section_name']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['section_name']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['section_name']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['section_name']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['section_name']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['section_name']['alter']['trim'] = 0;
$handler->display->display_options['fields']['section_name']['alter']['html'] = 0;
$handler->display->display_options['fields']['section_name']['hide_empty'] = 0;
$handler->display->display_options['fields']['section_name']['empty_zero'] = 0;
/* Field: Course enrolments: Start of enrolment */
$handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
$handler->display->display_options['fields']['timestamp']['table'] = 'course_enrolment';
$handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
$handler->display->display_options['fields']['timestamp']['relationship'] = 'course_enrollment';
$handler->display->display_options['fields']['timestamp']['label'] = 'Enrolled date';
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
$handler->display->display_options['defaults']['filters'] = FALSE;
/* Filter: Course report: Complete */
$handler->display->display_options['filters']['complete']['id'] = 'complete';
$handler->display->display_options['filters']['complete']['table'] = 'course_report';
$handler->display->display_options['filters']['complete']['field'] = 'complete';
/* Filter: Course: Transcript */
$handler->display->display_options['filters']['transcript']['id'] = 'transcript';
$handler->display->display_options['filters']['transcript']['table'] = 'course_node';
$handler->display->display_options['filters']['transcript']['field'] = 'transcript';
$handler->display->display_options['filters']['transcript']['relationship'] = 'nid';
$handler->display->display_options['filters']['transcript']['value'] = '1';
$handler->display->display_options['filters']['transcript']['expose']['operator'] = FALSE;
$handler->display->display_options['path'] = 'user/%/activities/pending';
$handler->display->display_options['menu']['type'] = 'tab';
$handler->display->display_options['menu']['title'] = 'Courses (in progress)';
$handler->display->display_options['menu']['weight'] = '0';

/* Display: Completed */
$handler = $view->new_display('page', 'Completed', 'page_2');
$handler->display->display_options['defaults']['title'] = FALSE;
$handler->display->display_options['title'] = 'Completed Activities';
$handler->display->display_options['defaults']['empty'] = FALSE;
/* Empty text: Global: Text area */
$handler->display->display_options['empty']['text']['id'] = 'area';
$handler->display->display_options['empty']['text']['table'] = 'views';
$handler->display->display_options['empty']['text']['field'] = 'area';
$handler->display->display_options['empty']['text']['label'] = '';
$handler->display->display_options['empty']['text']['empty'] = FALSE;
$handler->display->display_options['empty']['text']['content'] = 'You don\'t have any completed activities.';
$handler->display->display_options['empty']['text']['format'] = '2';
$handler->display->display_options['defaults']['relationships'] = FALSE;
/* Relationship: Course report: Course Node */
$handler->display->display_options['relationships']['nid']['id'] = 'nid';
$handler->display->display_options['relationships']['nid']['table'] = 'course_report';
$handler->display->display_options['relationships']['nid']['field'] = 'nid';
$handler->display->display_options['relationships']['nid']['required'] = 1;
/* Relationship: Course report: Course report user */
$handler->display->display_options['relationships']['uid']['id'] = 'uid';
$handler->display->display_options['relationships']['uid']['table'] = 'course_report';
$handler->display->display_options['relationships']['uid']['field'] = 'uid';
/* Relationship: Course report: Course enrollment */
$handler->display->display_options['relationships']['course_enrollment']['id'] = 'course_enrollment';
$handler->display->display_options['relationships']['course_enrollment']['table'] = 'course_report';
$handler->display->display_options['relationships']['course_enrollment']['field'] = 'course_enrollment';
/* Relationship: Course report: Credit awarded */
$handler->display->display_options['relationships']['credit_awarded']['id'] = 'credit_awarded';
$handler->display->display_options['relationships']['credit_awarded']['table'] = 'course_report';
$handler->display->display_options['relationships']['credit_awarded']['field'] = 'credit_awarded';
/* Relationship: Course credit awarded: Credit type */
$handler->display->display_options['relationships']['ctid_global']['id'] = 'ctid_global';
$handler->display->display_options['relationships']['ctid_global']['table'] = 'course_credit_awarded';
$handler->display->display_options['relationships']['ctid_global']['field'] = 'ctid_global';
$handler->display->display_options['relationships']['ctid_global']['relationship'] = 'credit_awarded';
$handler->display->display_options['relationships']['ctid_global']['required'] = 0;
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Node: Nid */
$handler->display->display_options['fields']['nid']['id'] = 'nid';
$handler->display->display_options['fields']['nid']['table'] = 'node';
$handler->display->display_options['fields']['nid']['field'] = 'nid';
$handler->display->display_options['fields']['nid']['relationship'] = 'nid';
$handler->display->display_options['fields']['nid']['exclude'] = TRUE;
$handler->display->display_options['fields']['nid']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['nid']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['nid']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['nid']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['nid']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['nid']['alter']['trim'] = 0;
$handler->display->display_options['fields']['nid']['alter']['html'] = 0;
$handler->display->display_options['fields']['nid']['hide_empty'] = 0;
$handler->display->display_options['fields']['nid']['empty_zero'] = 0;
$handler->display->display_options['fields']['nid']['link_to_node'] = 0;
/* Field: Node: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['relationship'] = 'nid';
$handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['title']['alter']['trim'] = 0;
$handler->display->display_options['fields']['title']['alter']['html'] = 0;
$handler->display->display_options['fields']['title']['hide_empty'] = 0;
$handler->display->display_options['fields']['title']['empty_zero'] = 0;
$handler->display->display_options['fields']['title']['link_to_node'] = 1;
/* Field: Course credit awarded: Credits */
$handler->display->display_options['fields']['credits']['id'] = 'credits';
$handler->display->display_options['fields']['credits']['table'] = 'course_credit_awarded';
$handler->display->display_options['fields']['credits']['field'] = 'credits';
$handler->display->display_options['fields']['credits']['relationship'] = 'credit_awarded';
$handler->display->display_options['fields']['credits']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['credits']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['credits']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['credits']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['credits']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['credits']['alter']['trim'] = 0;
$handler->display->display_options['fields']['credits']['alter']['html'] = 0;
$handler->display->display_options['fields']['credits']['hide_empty'] = 0;
$handler->display->display_options['fields']['credits']['empty_zero'] = 0;
$handler->display->display_options['fields']['credits']['set_precision'] = 1;
$handler->display->display_options['fields']['credits']['precision'] = '2';
/* Field: Course credit type: Title */
$handler->display->display_options['fields']['title_1']['id'] = 'title_1';
$handler->display->display_options['fields']['title_1']['table'] = 'course_credit_type';
$handler->display->display_options['fields']['title_1']['field'] = 'title';
$handler->display->display_options['fields']['title_1']['relationship'] = 'ctid_global';
$handler->display->display_options['fields']['title_1']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['title_1']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['title_1']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['title_1']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['title_1']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['title_1']['alter']['trim'] = 0;
$handler->display->display_options['fields']['title_1']['alter']['html'] = 0;
$handler->display->display_options['fields']['title_1']['hide_empty'] = 0;
$handler->display->display_options['fields']['title_1']['empty_zero'] = 0;
/* Field: Course report: Course status */
$handler->display->display_options['fields']['section_name']['id'] = 'section_name';
$handler->display->display_options['fields']['section_name']['table'] = 'course_report';
$handler->display->display_options['fields']['section_name']['field'] = 'section_name';
$handler->display->display_options['fields']['section_name']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['section_name']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['section_name']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['section_name']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['section_name']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['section_name']['alter']['trim'] = 0;
$handler->display->display_options['fields']['section_name']['alter']['html'] = 0;
$handler->display->display_options['fields']['section_name']['hide_empty'] = 0;
$handler->display->display_options['fields']['section_name']['empty_zero'] = 0;
/* Field: Course enrolments: Start of enrolment */
$handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
$handler->display->display_options['fields']['timestamp']['table'] = 'course_enrolment';
$handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
$handler->display->display_options['fields']['timestamp']['relationship'] = 'course_enrollment';
$handler->display->display_options['fields']['timestamp']['label'] = 'Enrolled date';
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
$handler->display->display_options['fields']['timestamp']['date_format'] = 'custom';
$handler->display->display_options['fields']['timestamp']['custom_date_format'] = 'm/d/Y';
/* Field: Course report: Course date completed */
$handler->display->display_options['fields']['date_completed']['id'] = 'date_completed';
$handler->display->display_options['fields']['date_completed']['table'] = 'course_report';
$handler->display->display_options['fields']['date_completed']['field'] = 'date_completed';
$handler->display->display_options['fields']['date_completed']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['date_completed']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['date_completed']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['date_completed']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['date_completed']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['date_completed']['alter']['trim'] = 0;
$handler->display->display_options['fields']['date_completed']['alter']['html'] = 0;
$handler->display->display_options['fields']['date_completed']['hide_empty'] = 0;
$handler->display->display_options['fields']['date_completed']['empty_zero'] = 0;
$handler->display->display_options['fields']['date_completed']['date_format'] = 'custom';
$handler->display->display_options['fields']['date_completed']['custom_date_format'] = 'm/d/Y';
/* Field: Course: Certificate */
$handler->display->display_options['fields']['certificate']['id'] = 'certificate';
$handler->display->display_options['fields']['certificate']['table'] = 'course_node';
$handler->display->display_options['fields']['certificate']['field'] = 'certificate';
$handler->display->display_options['fields']['certificate']['relationship'] = 'nid';
$handler->display->display_options['fields']['certificate']['alter']['alter_text'] = 1;
$handler->display->display_options['fields']['certificate']['alter']['text'] = 'Download';
$handler->display->display_options['fields']['certificate']['alter']['make_link'] = 1;
$handler->display->display_options['fields']['certificate']['alter']['path'] = 'node/[nid]/certificate';
$handler->display->display_options['fields']['certificate']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['certificate']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['certificate']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['certificate']['alter']['trim'] = 0;
$handler->display->display_options['fields']['certificate']['alter']['html'] = 0;
$handler->display->display_options['fields']['certificate']['hide_empty'] = 1;
$handler->display->display_options['fields']['certificate']['empty_zero'] = 1;
$handler->display->display_options['defaults']['filters'] = FALSE;
/* Filter: Course report: Complete */
$handler->display->display_options['filters']['complete']['id'] = 'complete';
$handler->display->display_options['filters']['complete']['table'] = 'course_report';
$handler->display->display_options['filters']['complete']['field'] = 'complete';
$handler->display->display_options['filters']['complete']['value'] = '1';
$handler->display->display_options['filters']['complete']['expose']['operator'] = FALSE;
/* Filter: Course: Transcript */
$handler->display->display_options['filters']['transcript']['id'] = 'transcript';
$handler->display->display_options['filters']['transcript']['table'] = 'course_node';
$handler->display->display_options['filters']['transcript']['field'] = 'transcript';
$handler->display->display_options['filters']['transcript']['relationship'] = 'nid';
$handler->display->display_options['filters']['transcript']['value'] = '1';
$handler->display->display_options['filters']['transcript']['expose']['operator'] = FALSE;
$handler->display->display_options['path'] = 'user/%/activities/completed';
$handler->display->display_options['menu']['type'] = 'default tab';
$handler->display->display_options['menu']['title'] = 'Transcript';
$handler->display->display_options['menu']['weight'] = '0';
$handler->display->display_options['tab_options']['type'] = 'tab';
$handler->display->display_options['tab_options']['title'] = 'My Activities';
$handler->display->display_options['tab_options']['weight'] = '0';
$handler->display->display_options['tab_options']['name'] = 'primary-links';

/* Display: Completed Activities Block */
$handler = $view->new_display('block', 'Completed Activities Block', 'block_1');
$handler->display->display_options['defaults']['title'] = FALSE;
$handler->display->display_options['title'] = 'Transcript';
$handler->display->display_options['defaults']['use_ajax'] = FALSE;
$handler->display->display_options['use_ajax'] = TRUE;
$handler->display->display_options['defaults']['link_display'] = FALSE;
$handler->display->display_options['link_display'] = 'page_2';
$handler->display->display_options['defaults']['pager'] = FALSE;
$handler->display->display_options['pager']['type'] = 'some';
$handler->display->display_options['pager']['options']['items_per_page'] = '4';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['defaults']['empty'] = FALSE;
/* Empty text: Global: Text area */
$handler->display->display_options['empty']['text']['id'] = 'area';
$handler->display->display_options['empty']['text']['table'] = 'views';
$handler->display->display_options['empty']['text']['field'] = 'area';
$handler->display->display_options['empty']['text']['label'] = '';
$handler->display->display_options['empty']['text']['empty'] = FALSE;
$handler->display->display_options['empty']['text']['content'] = '<p>You do not have any completed activities in your transcript at this time.</p>';
$handler->display->display_options['empty']['text']['format'] = '2';
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Node: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['relationship'] = 'nid';
$handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['title']['alter']['trim'] = 0;
$handler->display->display_options['fields']['title']['alter']['html'] = 0;
$handler->display->display_options['fields']['title']['hide_empty'] = 0;
$handler->display->display_options['fields']['title']['empty_zero'] = 0;
$handler->display->display_options['fields']['title']['link_to_node'] = 1;
/* Field: Course report: Credits */
$handler->display->display_options['fields']['cecredits']['id'] = 'cecredits';
$handler->display->display_options['fields']['cecredits']['table'] = 'course_report';
$handler->display->display_options['fields']['cecredits']['field'] = 'cecredits';
$handler->display->display_options['fields']['cecredits']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['cecredits']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['cecredits']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['cecredits']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['cecredits']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['cecredits']['alter']['trim'] = 0;
$handler->display->display_options['fields']['cecredits']['alter']['html'] = 0;
$handler->display->display_options['fields']['cecredits']['hide_empty'] = 0;
$handler->display->display_options['fields']['cecredits']['empty_zero'] = 0;
/* Field: Course report: Max credits */
$handler->display->display_options['fields']['cecreditsmax']['id'] = 'cecreditsmax';
$handler->display->display_options['fields']['cecreditsmax']['table'] = 'course_report';
$handler->display->display_options['fields']['cecreditsmax']['field'] = 'cecreditsmax';
$handler->display->display_options['fields']['cecreditsmax']['label'] = 'Max Credits';
$handler->display->display_options['fields']['cecreditsmax']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['cecreditsmax']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['cecreditsmax']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['cecreditsmax']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['cecreditsmax']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['cecreditsmax']['alter']['trim'] = 0;
$handler->display->display_options['fields']['cecreditsmax']['alter']['html'] = 0;
$handler->display->display_options['fields']['cecreditsmax']['hide_empty'] = 0;
$handler->display->display_options['fields']['cecreditsmax']['empty_zero'] = 0;
/* Field: Course report: Credits claimed */
$handler->display->display_options['fields']['credits_claimed']['id'] = 'credits_claimed';
$handler->display->display_options['fields']['credits_claimed']['table'] = 'course_report';
$handler->display->display_options['fields']['credits_claimed']['field'] = 'credits_claimed';
$handler->display->display_options['fields']['credits_claimed']['label'] = 'Claimed Credits';
$handler->display->display_options['fields']['credits_claimed']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['credits_claimed']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['credits_claimed']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['credits_claimed']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['credits_claimed']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['credits_claimed']['alter']['trim'] = 0;
$handler->display->display_options['fields']['credits_claimed']['alter']['html'] = 0;
$handler->display->display_options['fields']['credits_claimed']['hide_empty'] = 0;
$handler->display->display_options['fields']['credits_claimed']['empty_zero'] = 0;
/* Field: Course report: Course status */
$handler->display->display_options['fields']['section_name']['id'] = 'section_name';
$handler->display->display_options['fields']['section_name']['table'] = 'course_report';
$handler->display->display_options['fields']['section_name']['field'] = 'section_name';
$handler->display->display_options['fields']['section_name']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['section_name']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['section_name']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['section_name']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['section_name']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['section_name']['alter']['trim'] = 0;
$handler->display->display_options['fields']['section_name']['alter']['html'] = 0;
$handler->display->display_options['fields']['section_name']['hide_empty'] = 0;
$handler->display->display_options['fields']['section_name']['empty_zero'] = 0;
/* Field: Course enrolments: Start of enrolment */
$handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
$handler->display->display_options['fields']['timestamp']['table'] = 'course_enrolment';
$handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
$handler->display->display_options['fields']['timestamp']['label'] = 'Enrolled date';
$handler->display->display_options['fields']['timestamp']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['timestamp']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['timestamp']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['trim'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['html'] = 0;
$handler->display->display_options['fields']['timestamp']['hide_empty'] = 0;
$handler->display->display_options['fields']['timestamp']['empty_zero'] = 0;
/* Field: Course report: Course date completed */
$handler->display->display_options['fields']['date_completed']['id'] = 'date_completed';
$handler->display->display_options['fields']['date_completed']['table'] = 'course_report';
$handler->display->display_options['fields']['date_completed']['field'] = 'date_completed';
$handler->display->display_options['fields']['date_completed']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['date_completed']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['date_completed']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['date_completed']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['date_completed']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['date_completed']['alter']['trim'] = 0;
$handler->display->display_options['fields']['date_completed']['alter']['html'] = 0;
$handler->display->display_options['fields']['date_completed']['hide_empty'] = 0;
$handler->display->display_options['fields']['date_completed']['empty_zero'] = 0;
$handler->display->display_options['defaults']['filters'] = FALSE;
/* Filter: Course report: Complete */
$handler->display->display_options['filters']['complete']['id'] = 'complete';
$handler->display->display_options['filters']['complete']['table'] = 'course_report';
$handler->display->display_options['filters']['complete']['field'] = 'complete';
$handler->display->display_options['filters']['complete']['value'] = '1';
$handler->display->display_options['filters']['complete']['expose']['operator'] = FALSE;
/* Filter: Course: Transcript */
$handler->display->display_options['filters']['transcript']['id'] = 'transcript';
$handler->display->display_options['filters']['transcript']['table'] = 'course_node';
$handler->display->display_options['filters']['transcript']['field'] = 'transcript';
$handler->display->display_options['filters']['transcript']['relationship'] = 'nid';
$handler->display->display_options['filters']['transcript']['value'] = '1';
$handler->display->display_options['filters']['transcript']['expose']['operator'] = FALSE;

/* Display: Pending Activities Block */
$handler = $view->new_display('block', 'Pending Activities Block', 'block_2');
$handler->display->display_options['defaults']['title'] = FALSE;
$handler->display->display_options['title'] = 'Pending Activites';
$handler->display->display_options['defaults']['use_more'] = FALSE;
$handler->display->display_options['use_more'] = TRUE;
$handler->display->display_options['defaults']['pager'] = FALSE;
$handler->display->display_options['pager']['type'] = 'some';
$handler->display->display_options['pager']['options']['items_per_page'] = '4';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'list';
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['row_plugin'] = 'fields';
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['defaults']['empty'] = FALSE;
/* Empty text: Global: Text area */
$handler->display->display_options['empty']['text']['id'] = 'area';
$handler->display->display_options['empty']['text']['table'] = 'views';
$handler->display->display_options['empty']['text']['field'] = 'area';
$handler->display->display_options['empty']['text']['label'] = '';
$handler->display->display_options['empty']['text']['empty'] = FALSE;
$handler->display->display_options['empty']['text']['content'] = '<p>You are not enrolled in any courses at this time.</p>';
$handler->display->display_options['empty']['text']['format'] = '2';
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Node: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['relationship'] = 'nid';
$handler->display->display_options['fields']['title']['label'] = '';
$handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['title']['alter']['trim'] = 0;
$handler->display->display_options['fields']['title']['alter']['html'] = 0;
$handler->display->display_options['fields']['title']['hide_empty'] = 0;
$handler->display->display_options['fields']['title']['empty_zero'] = 0;
$handler->display->display_options['fields']['title']['link_to_node'] = 1;
/* Field: Course report: Credits */
$handler->display->display_options['fields']['cecredits']['id'] = 'cecredits';
$handler->display->display_options['fields']['cecredits']['table'] = 'course_report';
$handler->display->display_options['fields']['cecredits']['field'] = 'cecredits';
$handler->display->display_options['fields']['cecredits']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['cecredits']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['cecredits']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['cecredits']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['cecredits']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['cecredits']['alter']['trim'] = 0;
$handler->display->display_options['fields']['cecredits']['alter']['html'] = 0;
$handler->display->display_options['fields']['cecredits']['hide_empty'] = 0;
$handler->display->display_options['fields']['cecredits']['empty_zero'] = 0;
/* Field: Course report: Max credits */
$handler->display->display_options['fields']['cecreditsmax']['id'] = 'cecreditsmax';
$handler->display->display_options['fields']['cecreditsmax']['table'] = 'course_report';
$handler->display->display_options['fields']['cecreditsmax']['field'] = 'cecreditsmax';
$handler->display->display_options['fields']['cecreditsmax']['label'] = 'Max Credits';
$handler->display->display_options['fields']['cecreditsmax']['exclude'] = TRUE;
$handler->display->display_options['fields']['cecreditsmax']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['cecreditsmax']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['cecreditsmax']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['cecreditsmax']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['cecreditsmax']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['cecreditsmax']['alter']['trim'] = 0;
$handler->display->display_options['fields']['cecreditsmax']['alter']['html'] = 0;
$handler->display->display_options['fields']['cecreditsmax']['hide_empty'] = 0;
$handler->display->display_options['fields']['cecreditsmax']['empty_zero'] = 0;
/* Field: Course report: Credits claimed */
$handler->display->display_options['fields']['credits_claimed']['id'] = 'credits_claimed';
$handler->display->display_options['fields']['credits_claimed']['table'] = 'course_report';
$handler->display->display_options['fields']['credits_claimed']['field'] = 'credits_claimed';
$handler->display->display_options['fields']['credits_claimed']['label'] = 'Claimed Credits';
$handler->display->display_options['fields']['credits_claimed']['exclude'] = TRUE;
$handler->display->display_options['fields']['credits_claimed']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['credits_claimed']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['credits_claimed']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['credits_claimed']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['credits_claimed']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['credits_claimed']['alter']['trim'] = 0;
$handler->display->display_options['fields']['credits_claimed']['alter']['html'] = 0;
$handler->display->display_options['fields']['credits_claimed']['hide_empty'] = 0;
$handler->display->display_options['fields']['credits_claimed']['empty_zero'] = 0;
/* Field: Course report: Course date completed */
$handler->display->display_options['fields']['date_completed']['id'] = 'date_completed';
$handler->display->display_options['fields']['date_completed']['table'] = 'course_report';
$handler->display->display_options['fields']['date_completed']['field'] = 'date_completed';
$handler->display->display_options['fields']['date_completed']['label'] = 'Date Completed';
$handler->display->display_options['fields']['date_completed']['exclude'] = TRUE;
$handler->display->display_options['fields']['date_completed']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['date_completed']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['date_completed']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['date_completed']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['date_completed']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['date_completed']['alter']['trim'] = 0;
$handler->display->display_options['fields']['date_completed']['alter']['html'] = 0;
$handler->display->display_options['fields']['date_completed']['hide_empty'] = 0;
$handler->display->display_options['fields']['date_completed']['empty_zero'] = 0;
/* Field: Course enrolments: Start of enrolment */
$handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
$handler->display->display_options['fields']['timestamp']['table'] = 'course_enrolment';
$handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
$handler->display->display_options['fields']['timestamp']['label'] = 'Enrolled date';
$handler->display->display_options['fields']['timestamp']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['timestamp']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['timestamp']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['trim'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['html'] = 0;
$handler->display->display_options['fields']['timestamp']['hide_empty'] = 0;
$handler->display->display_options['fields']['timestamp']['empty_zero'] = 0;
