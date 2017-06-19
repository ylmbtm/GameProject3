/*
   Copyright (c) 2011, Oracle and/or its affiliates. All rights reserved.










   The lines above are intentionally left blank
*/

/*
  Definitions private to the server,
  used in the networking layer to notify specific events.
*/

#ifndef _mysql_com_server_h
#define _mysql_com_server_h

struct st_net_server;

typedef void (*before_header_callback_fn)
  (struct st_net *net, void *user_data, size_t count);

typedef void (*after_header_callback_fn)
  (struct st_net *net, void *user_data, size_t count, my_bool rc);

struct st_net_server
{
  before_header_callback_fn m_before_header;
  after_header_callback_fn m_after_header;
  void *m_user_data;
};

typedef struct st_net_server NET_SERVER;

#endif
