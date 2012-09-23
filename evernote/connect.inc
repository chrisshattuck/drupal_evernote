<?php

  
  define('EVERNOTE_OAUTH_CONSUMER_KEY', 'stompeers-web');
  define('EVERNOTE_OAUTH_CONSUMER_SECRET', 'f96324581a773886');
  
  // Replace this value with https://www.evernote.com to use Evernote's production server
  //define('EVERNOTE_SERVER', 'https://sandbox.evernote.com');
  define('EVERNOTE_SERVER', 'https://www.evernote.com');

  // Replace this value with www.evernote.com to use Evernote's production server
  //define('EVERNOTE_NOTESTORE_HOST', 'sandbox.evernote.com');
  define('EVERNOTE_NOTESTORE_HOST', 'www.evernote.com');
  define('EVERNOTE_NOTESTORE_PORT', '443');
  define('EVERNOTE_NOTESTORE_PROTOCOL', 'https');  
  
  // Evernote server URLs. You should not need to change these values.
  define('EVERNOTE_REQUEST_TOKEN_URL', EVERNOTE_SERVER . '/oauth');
  define('EVERNOTE_ACCESS_TOKEN_URL', EVERNOTE_SERVER . '/oauth');
  define('EVERNOTE_AUTHORIZATION_URL', EVERNOTE_SERVER . '/OAuth.action');
  
  // Include the Evernote API from the lib subdirectory. 
  // lib simply contains the contents of /php/lib from the Evernote API SDK
  define("EVERNOTE_LIBS", dirname(__FILE__) . DIRECTORY_SEPARATOR . "lib");
  ini_set("include_path", ini_get("include_path") . PATH_SEPARATOR . EVERNOTE_LIBS);

  require_once("Thrift.php");
  require_once("transport/TTransport.php");
  require_once("transport/THttpClient.php");
  require_once("protocol/TProtocol.php");
  require_once("protocol/TBinaryProtocol.php");
  require_once("packages/Types/Types_types.php");
  require_once("packages/UserStore/UserStore.php");
  require_once("packages/NoteStore/NoteStore.php");
  
  
  /*
  // Verify that you successfully installed the PHP OAuth Extension
  if (!class_exists('OAuth')) {
    die("<span style=\"color:red\">The PHP OAuth Extension is not installed</span>");
  }

  // Verify that you have configured your API key
  if (strlen(EVERNOTE_OAUTH_CONSUMER_KEY) == 0 || strlen(EVERNOTE_OAUTH_CONSUMER_SECRET) == 0) {
    $configFile = dirname(__FILE__) . '/config.php';
    die("<span style=\"color:red\">Before using this sample code you must edit the file $configFile " .
        "and fill in EVERNOTE_OAUTH_CONSUMER_KEY and EVERNOTE_OAUTH_CONSUMER_SECRET with the values that you received from Evernote. " .
        "If you do not have an API key, you can request one from " .
        "<a href=\"http://dev.evernote.com/documentation/cloud/\">http://dev.evernote.com/documentation/cloud/</a></span>");
  }
  */
  
  

  /*
   * The first step of OAuth authentication: the client (this application) 
   * obtains temporary credentials from the server (Evernote). 
   *
   * After successfully completing this step, the client has obtained the
   * temporary credentials identifier, an opaque string that is only meaningful 
   * to the server, and the temporary credentials secret, which is used in 
   * signing the token credentials request in step 3.
   *
   * This step is defined in RFC 5849 section 2.1:
   * http://tools.ietf.org/html/rfc5849#section-2.1
   *
   * @return boolean TRUE on success, FALSE on failure
   */
  function getTemporaryCredentials() {
    global $lastError, $currentStatus;
    try {
      $oauth = new OAuth(EVERNOTE_OAUTH_CONSUMER_KEY, EVERNOTE_OAUTH_CONSUMER_SECRET);
      $requestTokenInfo = $oauth->getRequestToken(EVERNOTE_REQUEST_TOKEN_URL, getCallbackUrl());
      if ($requestTokenInfo) {
        variable_set('evernote_requestToken', $requestTokenInfo['oauth_token']);
        variable_set('evernote_requestTokenSecret', $requestTokenInfo['oauth_token_secret']);
        $currentStatus = 'Obtained temporary credentials';
        return TRUE;
      } else {
        $lastError = 'Failed to obtain temporary credentials: ' . $oauth->getLastResponse();
      }
    } catch (OAuthException $e) {
      $lastError = 'Error obtaining temporary credentials: ' . $e->getMessage();
    }
    return false;
  }


  /*
   * The third and final step in OAuth authentication: the client (this application)
   * exchanges the authorized temporary credentials for token credentials.
   *
   * After successfully completing this step, the client has obtained the
   * token credentials that are used to authenticate to the Evernote API.
   * In this sample application, we simply store these credentials in the user's
   * session. A real application would typically persist them.
   *
   * This step is defined in RFC 5849 section 2.3:
   * http://tools.ietf.org/html/rfc5849#section-2.3
   *
   * @return boolean TRUE on success, FALSE on failure
   */
  function getTokenCredentials() {
    global $lastError, $currentStatus;
    
    if (variable_get('evernote_accessToken', '') != '') {
      $lastError = 'Temporary credentials may only be exchanged for token credentials once';
      return FALSE;
    }
    
    try {
      $oauth = new OAuth(EVERNOTE_OAUTH_CONSUMER_KEY, EVERNOTE_OAUTH_CONSUMER_SECRET);
      $oauth->setToken(variable_get('evernote_requestToken', ''), variable_get('evernote_requestTokenSecret', ''));
      $accessTokenInfo = $oauth->getAccessToken(EVERNOTE_ACCESS_TOKEN_URL, null, variable_get('evernote_oauthVerifier', ''));
      if ($accessTokenInfo) {
        //$_SESSION['accessToken'] = $accessTokenInfo['oauth_token'];
        variable_set('evernote_accessToken', $accessTokenInfo['oauth_token']);
        //$_SESSION['accessTokenSecret'] = $accessTokenInfo['oauth_token_secret'];
        variable_set('evernote_accessTokenSecret', $accessTokenInfo['oauth_token_secret']);
        //$_SESSION['noteStoreUrl'] = $accessTokenInfo['edam_noteStoreUrl'];
        variable_set('evernote_noteStoreUrl', $accessTokenInfo['edam_noteStoreUrl']);
        //$_SESSION['webApiUrlPrefix'] = $accessTokenInfo['edam_webApiUrlPrefix'];
        variable_set('evernote_webApiUrlPrefix', $accessTokenInfo['edam_webApiUrlPrefix']);
        // The expiration date is sent as a Java timestamp - milliseconds since the Unix epoch
        //$_SESSION['tokenExpires'] = (int)($accessTokenInfo['edam_expires'] / 1000);
        variable_set('evernote_tokenExpires', (int)($accessTokenInfo['edam_expires'] / 1000));
        //$_SESSION['userId'] = $accessTokenInfo['edam_userId'];
        variable_set('evernote_userId', $accessTokenInfo['edam_userId']);
        $currentStatus = 'Exchanged the authorized temporary credentials for token credentials';
        return TRUE;
      } else {
        $lastError = 'Failed to obtain token credentials: ' . $oauth->getLastResponse();
      }
    } catch (OAuthException $e) {
      $lastError = 'Error obtaining token credentials: ' . $e->getMessage();
    }  
    return FALSE;
  }
   
  /*
   * Reset the current session.
   */
  function resetSession() {
    variable_del('evernote_requestToken');
    variable_del('evernote_oauthVerifier');
    variable_del('evernote_accessToken');
    variable_del('evernote_accessTokenSecret');
    variable_del('evernote_noteStoreUrl');
    variable_del('evernote_webApiUrlPrefix');
    variable_del('evernote_userId');
    variable_del('evernote_tokenExpires');
  }
  
  /*
   * Get the URL of this application. This URL is passed to the server (Evernote)
   * while obtaining unauthorized temporary credentials (step 1). The resource owner 
   * is redirected to this URL after authorizing the temporary credentials (step 2).
   */
  function getCallbackUrl() {
    $thisUrl = (empty($_SERVER['HTTPS'])) ? "http://" : "https://";
    $thisUrl .= $_SERVER['SERVER_NAME'];
    $thisUrl .= ($_SERVER['SERVER_PORT'] == 80 || $_SERVER['SERVER_PORT'] == 443) ? "" : (":".$_SERVER['SERVER_PORT']);
    //$thisUrl .= $_SERVER['SCRIPT_NAME'];
    $thisUrl .= '/admin/settings/evernote';
    $thisUrl .= '?action=callback';
    return $thisUrl;
  }
  
  /*
   * Get the Evernote server URL used to authorize unauthorized temporary credentials.
   */
  function getAuthorizationUrl() {
    $url = EVERNOTE_AUTHORIZATION_URL;
    $url .= '?oauth_token=';
    $url .= urlencode(variable_get('evernote_requestToken', ''));
    return $url;
  }  