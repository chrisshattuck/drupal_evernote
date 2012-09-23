// $Id: README.txt,v 1.1 2010/07/07 21:54:15 stompeers Exp $

CONTENTS OF THIS FILE
---------------------

* Introduction
* Installation
* Usage


INTRODUCTION
---------------------

Creator: ChrisShattuck <http://drupal.org/user/166383>

The original idea behind the Evernote module was to integrate the pulling
of content from Evernote for rapid blogging purposes. My hope is to further
abstract out some of the tools used to do this so that the module can be
used as an API for open-ended integration with Evernote.

This module will do the following:

* Establish a connection with an Evernote account
* Allow an administrator to set up a 'feed' that will perform a one-way
sync from Evernote to Drupal based on multiple criteria. For example, you
can set up a feed to pull content with a particular tag.
* Allow 2-way syncing of 'tags' as taxonomy terms
* Downloading images and other resources from Evernote notes into image or
file fields
* And a bunch more


INSTALLATION
---------------------

See http://drupal.org/getting-started/install-contrib for instructions on
how to install or update Drupal modules.

You will also need to add the Evernote API library to Drupal. Right now the
API isn't quite set up for web applications like this, so you'll need to do
some finagling. You can use the instructions below to build the library
yourself, or you can go to this URL to read about how to download a ZIP
file with this already done: (@TODO: add url here).

1. Download the Evernote API (labeled "Evernote API") -
http://www.evernote.com/about/developer/api/
2. Expand the zip
3. Copy the folder /sample/php/EDAMWebTest/lib/HTTP to /lib/php
4. Copy the folder /sample/php/EDAMWebTest/lib/OAuth to /lib/php
5. Next we need a few PEAR libraries that might not be installed on your
server, so...
6. Download the HTTP_Request PEAR script and add to /lib/php/HTTP -
http://pear.php.net/package/HTTP_Request/
7. Download the Net_URL PEAR script and add to /lib/php/HTTP -
http://pear.php.net/package/Net_URL/
8. Download the Net_Socket PEAR script and add to /lib/php/HTTP
-http://pear.php.net/package/Net_Socket/
9. Rename the folder /lib/php to evernoteand move it to your
/sites/all/libraries/folder


USAGE
---------------------

1. Go to /admin/build/module and click the 'create a new account' link
2. If you don't mind saving your Evernote username and password on the
server, fill in the username and password at the bottom of the form. To
receive a temporary authentication from Evernote, fill in the widget.
3. Once the account as been added, you can click on 'Add feed' next to the
account to create a feed.
4. Fill out the form to create the criteria for the feed. When cron runs,
it will import any new matches and update any changed notes.