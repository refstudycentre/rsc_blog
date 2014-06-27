<?php


/**
 * (re)set content type settings
 */
function rsc_blog_content_type_settings() {
  
  $type = 'rsc_blog';
  
  // settings from the content type manage page
  variable_set("node_preview_{$type}", 1);                  // Optional preview
  variable_set("node_options_{$type}", array());           // default unpublished
  variable_set("node_submitted_{$type}", 1);                // 'Display author and date information'.
  variable_set("comment_{$type}", COMMENT_NODE_OPEN);       // open comments
  variable_set("menu_options_{$type}", array());           // no available menus
  
}


/**
 * (re)create fields
 */
function rsc_blog_field_config($form_values = NULL) {
  
  $fieldnames = array_keys(field_info_fields());
  $prefix = 'rscb'; // RSC Blog

  // body text
  $fieldname = "{$prefix}_body";
  if (!in_array($fieldname,$fieldnames)) {
    field_create_field(array(
      'field_name'  => $fieldname,
      'type'        => 'text_long',
      'cardinality' => 1,
    ));
  }
  
  if (!empty($form_values['rsc_blog'])) {
    $settings = $form_values['rsc_blog'];        // use new settings
  } else {
    $settings = variable_get("rsc_blog",NULL);  // use old settings if any
  }
  
  // tag reference field
  if (!empty($settings) && !empty($settings['vocabs']['tag']) && $settings['vocabs']['tag'] > 0) {
    $tag_vocab = taxonomy_vocabulary_load($settings['vocabs']['tag']);
  }
  
  if (!empty($tag_vocab)) {                             // if a vocabulary has been configured
    $fieldname = "{$prefix}_tag_{$tag_vocab->machine_name}"; // determine what this field should be called
    if (strlen($fieldname) <= 32) {
      if (!in_array($fieldname, $fieldnames)) {          // create it if it doesn't exist yet
        field_create_field(array(
          'field_name'  => $fieldname,
          'type'        => 'taxonomy_term_reference',
          'cardinality' => FIELD_CARDINALITY_UNLIMITED,
          'settings'    => array(
            'allowed_values' => array(
              array(
                'vocabulary' => $tag_vocab->machine_name,
                'parent'     => '0',
              ),
            ),
          ),
        ));
        $fieldnames[] = $fieldname; // add field to list of names to prevent duplicate creation attempts
      }
    } else {
      drupal_set_message(t('Cannot create field @fieldname (more than 32 characters).', array('@fieldname' => $fieldname)),'error');
    }
  } else {
    drupal_set_message(t('Could not create fields for rsc_blog, because vocabulary settings have not been configured yet.'), 'error');
  }
  
}


/**
 * (re)configure field instances
 */
function rsc_blog_field_instance_config($form_values = NULL) {
  $t = get_t(); // this function might run during install, or any other time
  $prefix = "rscb";
  $fields = array_keys(field_info_fields());
  $instances = array_keys(field_info_instances('node','rsc_blog'));
  
  if (!empty($form_values['rsc_blog'])) {
    $settings = $form_values['rsc_blog'];        // use new settings
  } else {
    $settings = variable_get("rsc_blog",NULL);  // use old settings if any
  }
  
  if (!empty($settings) && !empty($settings['vocabs']['tag']) && $settings['vocabs']['tag'] > 0) {
    $tag_vocab = taxonomy_vocabulary_load($settings['vocabs']['tag']);
  }
  
  // Tags term reference
  if (!empty($tag_vocab)) {                                 // if tag vocabulary has been configured
    $fieldname = "{$prefix}_tag_{$tag_vocab->machine_name}"; // determine what this field is called
    if (in_array($fieldname, $fields)) {                     // if the field exists
  
      // rsc_blog:
      if (!in_array($fieldname, $instances)) { // if the instance does not exist yet
        field_create_instance(array(               // create the instance
          'field_name'  => $fieldname,
          'entity_type' => 'node',
          'bundle'      => 'rsc_blog',
          'label'       => $t('Tags'),
          'required'    => false,
          'display'     => array(
            'default' => array(
              'type' => 'hidden',
            ),
          ),
          'widget'      => array('type' => 'options_select'),
        ));
        $instances[] = $fieldname;
      }
  
    } else {
      drupal_set_message(t('Cannot create instances of non-existing field <strong>@fieldname</strong> on content type <strong>@type</strong>.',array('@fieldname'=>$fieldname,'@type'=>'rsc_blog')),'error');
    }
  }

  /* Body text */
  $fieldname = "{$prefix}_body";
  
  // rsc_blog:
  if (!in_array($fieldname, $instances) && in_array($fieldname, $fields)) {
    field_create_instance(array(
      'field_name'  => $fieldname,
      'entity_type' => 'node',
      'bundle'      => 'rsc_blog',
      'label'       => $t('Body'),
      'required'    => false,
      'settings'    => array('text_processing' => 1), // allow text processing
      'display'     => array(
        'default' => array(
          'label'  => 'hidden',
          'weight' => 1,
        ),
      ),
    ));
    $instances[] = $fieldname;
  }
  
}


/**
 * Delete unused fields that start with rscb_
 * Unused means: does not have any instances
 */
function rsc_blog_field_cleanup() {
  $fields = array_keys(field_info_fields());
  $used_fields = array_keys(field_info_field_map());
  $unused_fields = array_diff($fields,$used_fields);

  foreach ($unused_fields as $field) {
    if (substr($field,0,5) === 'rscb_') {
      field_delete_field($field);
    }
  }
}

