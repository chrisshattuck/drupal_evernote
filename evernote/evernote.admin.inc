<?php


function evernote_admin_settings_form() {
  evernote_connect();
  
  if ($_GET['sync']) {
    evernote_sync(1000);
    drupal_set_message('We just synced your Evernote, you should be good to go!');
    drupal_goto('admin/settings/evernote');
  }
  
  //resetSession();
  try {
    $noteStore = evernote_get_notestore();
    $authToken = variable_get('evernote_accessToken', '');
    $notebooks = $noteStore->listNotebooks($authToken);
    foreach ($notebooks as $notebook) {
      $text .= $notebook->name . ' ' . $notebook->guid;
      $options[$notebook->guid] = $notebook->name;
    }
    $form['evernote_notebook'] = array(
      '#type' => 'select',
      '#options' => $options,
      '#title' => t('Select a notebook to sync with this site'),
      '#default_value' => variable_get('evernote_notebook', ''),
    );
    
    $types = node_get_types();
    foreach($types as $type) {
      $type_options[$type->type] = $type->name .' ('. $type->type .')';
    }
    $form['evernote_content_type'] = array(
      '#type' => 'select',
      '#title' => t('Content type'),
      '#options' => $type_options,
      '#description' => t('The node type that evernote notes will be added as.'),
      '#default_value' => variable_get('evernote_content_type', ''),
    );
    
    
    $fields = content_fields();
    $imagefield_options[''] = '';
    $filefield_options[''] = '';
    foreach ($fields as $key => $field) {
      if ($field['type'] == 'text') {
        $field_options[$key] = $field['widget']['label'] .' ('. $key .')';
      }
      if ($field['type'] == 'filefield' && $field['widget']['module'] != 'imagefield') {
        $filefield_options[$key] = $field['widget']['label'] .' ('. $key .')';
      }
      if ($field['type'] == 'filefield' && $field['widget']['module'] == 'imagefield') {
        $imagefield_options[$key] = $field['widget']['label'] .' ('. $key .')';
      }
    }
    $title_field_options = array('title' => 'Title');
    if (is_array($field_options)) {
      $title_field_options = array_merge($title_field_options, $field_options);
    }
    $form['evernote_title_field'] = array(
      '#type' => 'select',
      '#title' => t('Title field'),
      '#options' => $title_field_options,
      '#default_value' => variable_get('evernote_title_field', 'title'),
      '#description' => t('Select the field where the title of the Evernote note will be entered. Note that even if you select a CCK field, the title will also be saved to the node title as well.'),
    );
    
    $form['fields']['evernote_body'] = array(
      '#type' => 'fieldset',
      '#title' => 'Body field',
    );
    
    $body_field_options = array('body' => 'Body');
    if (is_array($field_options)) {
      $body_field_options = array_merge($body_field_options, $field_options);
    }
    $form['fields']['evernote_body']['evernote_body_field'] = array(
      '#type' => 'select',
      '#options' => $body_field_options,
      '#default_value' => variable_get('evernote_body_field', 'body'),
      '#description' => t('Select the field where the content of the Evernote note will be entered. Note that even if you select a CCK field, the content will also be saved to the node body as well.'),
    );
    $default_format = variable_get('evernote_body_format', '');
    $form['fields']['evernote_body']['evernote_body_format'] = filter_form($default_format, '10', array('evernote_body_format'));
    
    
    $form['fields']['evernote_image_field'] = array(
      '#title' => t('Image field'),
      '#type' => 'select',
      '#options' => $imagefield_options,
      '#default_value' => variable_get('evernote_image_field', ''),
      '#description' => t('Select the field where associated images will be stored.'),
    );
    $form['fields']['evernote_file_field'] = array(
      '#title' => t('File field'),
      '#type' => 'select',
      '#options' => $filefield_options,
      '#default_value' => variable_get('evernote_file_field', ''),
      '#description' => t('Select the field where associated non-image files will be stored.'),
    );
  
    
    //$add_as = user_load(array('uid' => variable_get('evernote_uid', 1)))->name;
    // @TODO: Change this to 'evernote_username'
    $form['fields']['evernote_uid'] = array(
      '#type' => 'textfield',
      '#title' => t('User to add new entries as'),
      '#size' => 30,
      '#maxlength' => 60,
      '#autocomplete_path' => 'user/autocomplete',
      '#default_value' => variable_get('evernote_uid', 1),
    );
    
    
    $ocr_field_options = array('' => '');
    if (is_array($field_options)) {
      $ocr_field_options = array_merge($ocr_field_options, $field_options);
    }
    $form['fields']['evernote_ocr_field'] = array(
      '#type' => 'select',
      '#title' => t('OCR text field'),
      '#options' => $ocr_field_options,
      '#default_value' => variable_get('evernote_ocr_field', ''),
      '#description' => t('Select the field where any text that has been OCR-ed from an image or other resource by Evernote will be stored.'),
    );
    
    
    $form['link'] = array(
      '#type' => 'markup',
      '#value' => '<p><a href="' . url('admin/settings/evernote', array('query' => 'sync=1')) . '">Do a large sync (cron syncs up only the last 100 updated notes, this will do the last 1000)</a>.</p>',
    );
    
  } catch (Exception $e) {
    if (getTemporaryCredentials()) {
      // We obtained temporary credentials, now redirect the user to evernote.com to authorize access
      $form['link'] = array(
        '#type' => 'markup',
        '#value' => '<p><a href="' . getAuthorizationUrl() . '">Click here to authorize connecting to your Evernote account</a>.</p>',
        );
    }
  }
  
  return system_settings_form($form);
}

 