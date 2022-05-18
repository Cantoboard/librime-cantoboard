# Cantoboard Rime plugin module

A Rime plugin providing additional functions for Cantoboard.
Due to how dylib is handled in iOS, it must be statically linked to librime.
To do that, create symlink under librime/plugins (e.g. librime/plugins/cantoboard) to point to this module.
Then build librime with `BUILD_MERGED_PLUGINS=ON`.
