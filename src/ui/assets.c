#include "assets.h"

#include <stdio.h>
#include <string.h>

#define ASSET_PATH_CAPACITY 1024

static AppAssets assets;

static const char *resolve_asset_path(const char *relative_path)
{
    static char path[ASSET_PATH_CAPACITY];
    const char *application_directory;
    size_t length;
    const char *separator;

    (void)snprintf(path, sizeof(path), "assets/%s", relative_path);
    if (FileExists(path)) {
        return path;
    }

    application_directory = GetApplicationDirectory();
    length = strlen(application_directory);
    separator = length > 0U &&
                        (application_directory[length - 1U] == '/' ||
                         application_directory[length - 1U] == '\\')
                    ? ""
                    : "/";

    (void)snprintf(path,
                   sizeof(path),
                   "%s%sassets/%s",
                   application_directory,
                   separator,
                   relative_path);
    return path;
}

static Texture2D load_texture(const char *relative_path)
{
    const char *path = resolve_asset_path(relative_path);
    Texture2D texture = LoadTexture(path);

    if (!IsTextureValid(texture)) {
        TraceLog(LOG_WARNING, "Asset introuvable ou invalide: %s", path);
    }
    return texture;
}

bool assets_load(void)
{
    if (!IsWindowReady()) {
        return false;
    }

    assets.apple = load_texture("sprites/apple.png");
    assets.trophy = load_texture("sprites/trophy.png");
    assets.portal = load_texture("sprites/portal.png");
    assets.bomb = load_texture("sprites/bomb.png");
    assets.menu_background = load_texture("backgrounds/menu.png");

    return IsTextureValid(assets.apple) && IsTextureValid(assets.trophy) &&
           IsTextureValid(assets.portal) && IsTextureValid(assets.bomb) &&
           IsTextureValid(assets.menu_background);
}

static void unload_texture(Texture2D texture)
{
    if (IsTextureValid(texture)) {
        UnloadTexture(texture);
    }
}

void assets_unload(void)
{
    unload_texture(assets.apple);
    unload_texture(assets.trophy);
    unload_texture(assets.portal);
    unload_texture(assets.bomb);
    unload_texture(assets.menu_background);
    assets = (AppAssets){0};
}

const AppAssets *assets_get(void)
{
    return &assets;
}
