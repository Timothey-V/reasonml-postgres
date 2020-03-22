module Messages = {
  type m = {
    idmessage: int,
    texte: string,
    date_envoi: string,
    idchannel: int,
    author: string,
  };
  let m = {
    let encode = ({idmessage, texte,date_envoi,idchannel,author}) => Ok((idmessage, texte,date_envoi,idchannel,author));
    let decode = ((idmessage, texte,date_envoi,idchannel,author)) => Ok({idmessage, texte,date_envoi,idchannel,author});
    let rep = Caqti_type.(tup5(int,string,string,int,string));
    Caqti_type.custom(~encode, ~decode, rep);
  };
};

let or_error = m =>
  switch%lwt (m) {
  | Ok(a) => Ok(a) |> Lwt.return
  | Error(e) => Error(Caqti_error.show(e)) |> Lwt.return
  };

open Config;

let connection_url = Environment.values.db_connection;

let pool =
  switch (
    Caqti_lwt.connect_pool(~max_size=10, Uri.of_string(connection_url))
  ) {
  | Ok(pool) => pool
  | Error(err) => failwith(Caqti_error.show(err))
  };

// Migrations

module M = {
  let migrate_query =
      Caqti_request.exec(
      Caqti_type.unit,
      {| CREATE TABLE forum_messages (
          idmessage   int   NOT NULL   PRIMARY KEY,
          texte   character varying,
          date_envoi   character varying,
          idchannel   int   NOT NULL,
          author   character varying   NOT NULL
          )
      |},
      );

    let rollback_query =
    Caqti_request.exec(Caqti_type.unit, "DROP TABLE forum_messages");
};

let migrate = () => {
  let migrate' = (module Db: Caqti_lwt.CONNECTION) =>
    Db.exec(M.migrate_query, ());

  Caqti_lwt.Pool.use(migrate', pool) |> or_error;
};

let rollback = () => {
  let rollback' = (module Db: Caqti_lwt.CONNECTION) =>
    Db.exec(M.rollback_query, ());

  Caqti_lwt.Pool.use(rollback', pool) |> or_error;
};

// Queries

module Q = {
  let add_message_query =
  Caqti_request.exec(
      Caqti_type.(tup5(int,string,string,int,string)), 
      "INSERT INTO forum_messages (idmessage,texte,date_envoi,idchannel,author) VALUES (?,?,?,?,?)",
  );

  let get_all_messages_channel =
  Caqti_request.collect(
      Caqti_type.int,
      Messages.m,
      "SELECT idmessage, texte, date_envoi, author FROM forum_messages where idchannel=?",
  );

};

let add_message = content => {
  let add' = (content, module Db: Caqti_lwt.CONNECTION) =>
    Db.exec(Q.add_message_query, content);

  Caqti_lwt.Pool.use(add'(content), pool) |> or_error;
};

let get_all = query => {
let get_all = (module Db: Caqti_lwt.CONNECTION) =>
  Db.fold(query, (row, acc) => [row, ...acc], (), []);

Caqti_lwt.Pool.use(get_all, pool) |> or_error;
};

let get_all_messages_channel = () => get_all(Q.get_all_messages_channel);
