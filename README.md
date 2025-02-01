
# About Namida

**DISCLAIMER: This project is for academic purposes only. The developers assume no responsibility for any illegal use or potential harm.**

## Requirements

- PHP >= 7.2
- cURL extension support
- GuzzleHTTP 

# Install

```bash
rm -rf data/nodes.json data/dead_providers.json
```

```bash
php index.php update-nodes
```

```bash
php index.php refine-nodes
```

```bash
php index.php start-bombing [refined] <email address>
```

```bash
    php index.php start-bombing email@example.com
```

```bash
    php index.php start-bombing refined email@example.com
```