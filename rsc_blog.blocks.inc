<?php


/**
 * Implements hook_block_info().
 */
function rsc_blog_block_info() {

  $blocks['menu'] = array(
    'info'  => t('RSC Blog: Blogs menu'),
    'cache' => DRUPAL_NO_CACHE,  // we cache the query result inside block_view
  );

  $blocks['latest'] = array(
    'info'  => t('RSC Blog: Latest blog posts'),
    'cache' => DRUPAL_NO_CACHE,  // we cache the query result inside block_view
  );
  
  $blocks['featured'] = array(
    'info'  => t('RSC Blog: Featured blog posts'),
    'cache' => DRUPAL_NO_CACHE,  // we cache the query result inside block_view
  );

  return $blocks;
}


/**
 * Implements hook_block_view().
 */
function rsc_blog_block_view($delta = '') {
  $block = array();
  
  switch ($delta) {
    case 'menu':
      
      
      
      break;
    case 'latest':

      
      
      break;
    case 'featured':

      
      
      break;
  }
    
  $block = array(
    'subject' => NULL,
    'content' => array(
      
    ),
  );
  
  return $block;
}

