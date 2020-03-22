open Lwt.Infix;

module D = Decoders_yojson.Basic.Decode;

type message = {
  idmessage: int,
  texte: string,
  date_envoi: string,
  idchannel: int,
  author: string,
};

let message_decoder: D.decoder(message) = (
  D.(
    field("idmessage", int)
    >>= (
      idmessage =>
        field("texte", string)
        >>= (
          texte =>
            field("date_envoi", string)
            >>= (
              date_envoi =>
                field("idchannel", int)
                >>= (
                  idchannel => 
                    field("author", string)
                    >>= ( author => succeed({idmessage, texte, date_envoi, idchannel,author}))
            )
        )
    )
  ):
    D.decoder(message)
);


type message_data = {data: message};

let message_data_decoder: D.decoder(message_data) = (
  D.(field("data", post_decoder) >>= (data => succeed({data: data}))):
    D.decoder(message_data)
);

type forum = {children: list(message_data)};

let forum_decoder: D.decoder(forum) = (
  D.(
    field("children", list(message_data_decoder))
    >>= (children => succeed({children: children}))
  ):
    D.decoder(forum)
);

type forum_data = {data: forum};

let forum_data_decoder: D.decoder(forum_data) = (
  D.(field("data", forum_decoder) >>= (data => succeed({data: data}))):
    D.decoder(forum_data)
);
