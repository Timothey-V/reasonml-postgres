// Messages

open Message;

let message: Graphql_lwt.Schema.typ(unit, option(message_data)) =
  Graphql_lwt.Schema.(
    obj("Message", ~doc="A forum message", ~fields=_ =>
      [
        field(
          "idmessage",
          ~doc="Unique post identifier",
          ~typ=non_null(int),
          ~args=Arg.[],
          ~resolve=(_info, p: message_data) =>
          p.data.idmessage
        ),
        field(
          "texte",
          ~typ=non_null(string),
          ~args=Arg.[],
          ~resolve=(_info, p: message_data) =>
          p.data.texte
        ),
        field(
          "date_envoi",
          ~typ=non_null(string),
          ~args=Arg.[],
          ~resolve=(_info, p: message_data) =>
          p.data.date_envoi
        ),
        field(
          "idchannel",
          ~typ=non_null(int),
          ~args=Arg.[],
          ~resolve=(_info, p: message_data) =>
          p.data.idchannel
        ),
        field(
          "author",
          ~typ=non_null(string),
          ~args=Arg.[],
          ~resolve=(_info, p: message_data) =>
          p.data.author
        ),
      ]
    )
  );

let forum: Graphql_lwt.Schema.typ(unit, option(forum)) =
  Graphql_lwt.Schema.(
    obj("Forum", ~doc="A Forum", ~fields=_ =>
      [
        field(
          "messages",
          ~doc="List of messages",
          ~typ=non_null(list(non_null(post))),
          ~args=Arg.[],
          ~resolve=(_info, p) =>
          p.children
        ),
      ]
    )
  );