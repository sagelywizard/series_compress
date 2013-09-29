-module(series_compress).
-export([compress_binary/1, decompress_binary/1, compress/1, decompress/1]).
-on_load(init/0).

init() ->
    ok = erlang:load_nif("./series_compress", 1).

compress(_Binary) ->
    exit(nif_library_not_loaded).

decompress(_Binary) ->
    exit(nif_library_not_loaded).

compress_binary(_Binary) ->
    exit(nif_library_not_loaded).

decompress_binary(_Binary) ->
    exit(nif_library_not_loaded).
