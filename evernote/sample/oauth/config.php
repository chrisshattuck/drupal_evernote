<?php

  /*
   * Copyright 2010-2012 Evernote Corporation.
   *
   * This file contains configuration information for Evernote's PHP OAuth samples.
   * Before running the sample code, you must change the client credentials below.
   */
   
  // Client credentials. Fill in these values with the consumer key and consumer secret 
  // that you obtained from Evernote. If you do not have an Evernote API key, you may
  // request one from http://dev.evernote.com/documentation/cloud/
  define('EVERNOTE_OAUTH_CONSUMER_KEY', 'stompeers-web');
  define('EVERNOTE_OAUTH_CONSUMER_SECRET', 'f96324581a773886');
  
  // Replace this value with https://www.evernote.com to use Evernote's production server
  define('EVERNOTE_SERVER', 'https://sandbox.evernote.com');

  // Replace this value with www.evernote.com to use Evernote's production server
  define('EVERNOTE_NOTESTORE_HOST', 'sandbox.evernote.com');
  define('EVERNOTE_NOTESTORE_PORT', '443');
  define('EVERNOTE_NOTESTORE_PROTOCOL', 'https');  
  
  // Evernote server URLs. You should not need to change these values.
  define('EVERNOTE_REQUEST_TOKEN_URL', EVERNOTE_SERVER . '/oauth');
  define('EVERNOTE_ACCESS_TOKEN_URL', EVERNOTE_SERVER . '/oauth');
  define('EVERNOTE_AUTHORIZATION_URL', EVERNOTE_SERVER . '/OAuth.action');
?>
