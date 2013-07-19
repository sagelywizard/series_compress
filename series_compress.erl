-module(series_compress).
-export([compress64/1, decompress64/1,
         int64_list_to_binary/1, double_list_to_binary/1]).
-on_load(init/0).

init() ->
    ok = erlang:load_nif("./series_compress", 1).

compress64(_Binary) ->
    exit(nif_library_not_loaded).

decompress64(_Binary) ->
    exit(nif_library_not_loaded).

int64_list_to_binary(_TimeSeries) ->
    exit(nif_library_not_loaded).

double_list_to_binary(_Binary) ->
    exit(nif_library_not_loaded).
