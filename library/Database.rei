module Messages = {
  type m = {
    idmessage: int,
    texte: string,
    date_envoi: string,
    idchannel: int,
    author: string,
  };
};

let migrate: unit => Lwt.t(result(unit, string));
let rollback: unit => Lwt.t(result(unit, string));
let seed:
  (~urls: list(string)=?, unit) => Lwt.t(list(Stdlib.result(unit, string)));

let get_all_messages_channel: unit => Lwt.t(result(list(Messages.m), string));