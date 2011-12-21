<?php

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Add to cart form'),
  'single' => TRUE,
  'description' => t('Include add to cart form as a pane'),
  'required context' => new ctools_context_required(t('Node'), 'node'),
  'category' => t('Node'),
);

/**
 * Render the custom content type.
 */
function uc_course_addtocart_content_type_render($subtype, $conf, $panel_args, $context) {
  // Get a shortcut to the node.
  $node = $context->data;

  // Return if not applicable
  if (empty($context) || empty($context->data) || uc_product_is_product($node) == FALSE) {
    return;
  }

  // Load information about the node type.
  $type = node_type_get_type($node->type);
  $module = $type->module;

  // Built the add to cart button
  if (function_exists('theme_' . $module . '_add_to_cart')) {
    // TODO Please change this theme call to use an associative array for the $variables parameter.
    $result = theme($module . '_add_to_cart', $node);
  }
  elseif (function_exists('theme_uc_product_add_to_cart')) {
    // TODO Please change this theme call to use an associative array for the $variables parameter.
    $result = theme('uc_product_add_to_cart', $node);
  }

  // Build the content type block.
  $block = new stdClass();
  $block->content = $result;
  return $block;
}

/**
 * Blank edit form for selecting context
 */
function uc_course_addtocart_content_type_edit_form(&$form, &$form_state) { }

/**
 * Returns the administrative title for a type.
 */
function uc_course_addtocart_content_type_admin_title($subtype, $conf, $context) {
  return t('"@s" add to cart form', array('@s' => $context->identifier));
}
