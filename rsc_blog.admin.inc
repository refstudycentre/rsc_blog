<?php


/**
 * Form builder for the module settings page
 */
function rsc_blog_settings() {
  $form = array();

  /* collect info from site */
  $vocabs = taxonomy_get_vocabularies();
  $vocab_options = array('-1' => t('None'));
  foreach($vocabs as $vid => $vocab) {
    $vocab_options[$vid] = $vocab->name;
  }

  /* collect info from this module */
  $default_settings = array(
    'vocabs' => array(
      'tag'    => '-1',
    ),
  );
  $settings = variable_get("rsc_blog",$default_settings);

  /* build the form */
  $form['#tree'] = TRUE; // save the whole thing as an array

  $form['rsc_blog']['links'] = array(
    '#type'   => 'markup',
    '#markup' => drupal_render($link_list),
    '#prefix' => '<div class="form-item">',
    '#suffix' => '</div>',
  );

  $form['rsc_blog']['vocabs'] = array(
    '#type'  => 'fieldset',
    '#title' => 'Taxonomy vocabulary to use for...',
  );

  $form['rsc_blog']['vocabs']['tag'] = array(
    '#type'          => 'select',
    '#required'      => FALSE,
    '#multiple'      => FALSE,
    '#title'         => t('tags'),
    '#description'   => t('A reference field to terms in this vocabulary will be created on the blog content type.'),
    '#default_value' => $settings['vocabs']['tag'],
    '#options'       => $vocab_options,
  );

  $form['rsc_blog']['actions'] = array(
    '#type'          => 'radios',
    '#title'         => 'Actions:',
    '#description'   => t('Choose an optional action to perform when submitting.'),
    '#default_value' => 'Nothing',
    '#options'       => array(
      0                     => 'Nothing',
      'content_type_reset'     => 'Reset the content type settings.',
      'fields_create'          => '(re)Create missing fields and instances',
      'fields_cleanup'         => 'Delete (clean up) unused fields',
    ),
    'content_type_reset' => array('#description' => t("Default settings include: unpublished by default, comments open, etc.")),
    'fields_create'      => array('#description' => t("Old fields and field instances will not be deleted automatically. If you are experiencing trouble with a field or field instance, delete it, and then choose this option. YOU WILL LOSE ALL VALUES CONTAINED IN EVERY INSTANCE OF A FIELD IF YOU DELETE IT.")),
    'fields_cleanup'     => array('#description' => t("This will delete all fields created by rsc_blog which do not have any instances (i.e. are not linked to a content type any more). YOU WILL LOSE ALL DATA IN THESE FIELDS.")),
  );

  $form['#submit'][] = 'rsc_blog_settings_submit';
  return system_settings_form($form);
}


/**
 * Submit handler for the module settings page
 */
function rsc_blog_settings_submit($form, &$form_state) {
  if (!empty($form_state['values']['rsc_blog'])) {
    $values = $form_state['values']['rsc_blog'];
    switch($values['actions']) {
      case 'content_type_reset':
        
        module_load_include('inc','rsc_blog','rsc_blog.common');
        rsc_blog_content_type_settings();
        drupal_set_message("Settings for content types rsc_blog_* have been reset.");
        
        break;
      case 'fields_cleanup':
        
        module_load_include('inc','rsc_blog','rsc_blog.common');
        rsc_blog_field_cleanup();
        drupal_set_message("Unused fields from rsc_blog have been deleted.");
        
        break;
      case 'fields_create':
        
        module_load_include('inc','rsc_blog','rsc_blog.common');
        rsc_blog_field_config($form_state['values']);
        rsc_blog_field_instance_config($form_state['values']);
        drupal_set_message("Fields and instances for rsc_blog have been created.");
        
        break;
    }
  }
}

