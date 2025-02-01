<?php


define('PRESERVE_EMPTY_PROVIDERS',false);
define('USE_NODES_COUNT',5000);
define('CONCURRENCY',50);
define('THREAD_POOL_SIZE',20);
define('SHODAN_API_KEY','');
define('ZOOMEYE_API_KEY','');
define('ZOOMEYE_PAGE_LIMIT',50);
define('PROXY','');
define('TIMEOUT',30);
define('PROVIDERS_JSON',__DIR__.'/data/providers.json');
define('DEAD_PROVIDERS_JSON',__DIR__.'/data/dead_providers.json');
define('NODES_JSON',__DIR__.'/data/nodes.json');
define('REFINED_NODES_JSON',__DIR__.'/data/refined_nodes.json');
define('TEST_DIR',__DIR__.'/test');