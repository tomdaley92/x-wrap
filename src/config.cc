#include "config.h"
#include <windows.h>
#include <stdio.h>
#include <shlobj.h> /* SHGetKnownFolderPath() */
#include <wchar.h> /* _wmkdir() */

void init_config(XWRAP_CONFIG *config) {
    /* defaults */
    config->start_on_login = 0;
    config->start_in_tray = 0;
    config->minimize_on_exit = 0;
}

int load_config(XWRAP_CONFIG *config) {
    FILE *file;
    int fileStatus;
    wchar_t filePath[MAX_PATH] = L"";
    init_config(config);

    int start_on_login;
    int start_in_tray;
    int minimize_on_exit;
    
    /* Dynamically get the unicode path to Roaming AppData folder */
    wchar_t *temp;
    long int result = SHGetKnownFolderPath(
        FOLDERID_RoamingAppData, 
        KF_FLAG_CREATE | KF_FLAG_INIT, 
        NULL,
        &temp);

    if (result != S_OK) {
        fprintf(stderr, 
            "Error: Unable to locate \\%%USER%%\\AppData\\Roaming directory.\n");
        return 1;
    }
    
    wcsncat_s(filePath, temp, MAX_PATH);

    /* we must free what is allocated from SHGetKnownFolderPath */
    CoTaskMemFree(temp);

    wcsncat_s(filePath, L"\\Xwrap", 6);
    _wmkdir(filePath);
    wcsncat_s(filePath, L"\\config.ini", 11);

    /* attempt to open a config file */
    fileStatus = _wfopen_s(&file, filePath, L"r");
    if (fileStatus) {
        fprintf(stderr, "Creating new config file.\n");
        return save_config(config);
    }

    /* load values from the config file */
    int num_fields_extracted = fscanf_s( 
        file,
        "start_on_login = %d\n"
        "start_in_tray = %d\n"
        "minimize_on_exit = %d\n",
        &start_on_login,
        &start_in_tray,
        &minimize_on_exit );

    if (num_fields_extracted < CONFIG_NUM_REQUIRED_FIELDS) {
        fprintf(stderr, "Config file is corrupt!\n"
            "Creating new config file with default values.\n");
        fclose(file);
        return save_config(config);
    }

    /* set the config flags */
    config->start_on_login = start_on_login != 0;
    config->start_in_tray = start_in_tray != 0;
    config->minimize_on_exit = minimize_on_exit != 0;

    /* print config to console */
    fprintf( stderr, 
        "Loaded config file %S\n"
        "start_on_login = %d\n"
        "start_in_tray = %d\n"
        "minimize_on_exit = %d\n",
        filePath,
        config->start_on_login,
        config->start_in_tray,
        config->minimize_on_exit );

    fclose(file);
    return 0;
}

int save_config(XWRAP_CONFIG *config) {
    FILE *file;
    int fileStatus;
    wchar_t filePath[MAX_PATH] = L"";

    int start_on_login;
    int start_in_tray;
    int minimize_on_exit;

    start_on_login = config->start_on_login ? 1 : 0;
    start_in_tray = config->start_in_tray ? 1 : 0;
    minimize_on_exit = config->minimize_on_exit ? 1 : 0;
    
    /* Dynamically get the unicode path to Roaming AppData folder */
    wchar_t *temp;
    long int result = SHGetKnownFolderPath(
        FOLDERID_RoamingAppData, 
        KF_FLAG_CREATE | KF_FLAG_INIT, 
        NULL,
        &temp);

    if (result != S_OK) {
        fprintf(stderr, 
            "Error: Unable to locate \\%%USER%%\\AppData\\Roaming directory.\n");
        return 1;
    }
    
    wcsncat_s(filePath, temp, MAX_PATH);

    /* free what is allocated from SHGetKnownFolderPath */
    CoTaskMemFree(temp);

    wcsncat_s(filePath, L"\\Xwrap", 6);
    _wmkdir(filePath);
    wcsncat_s(filePath, L"\\config.ini", 11);

    /* overwrite the current config file */
    fileStatus = _wfopen_s(&file, filePath, L"w");
    if (fileStatus) {
        fprintf(stderr, "Failed to save config file!\n");
        return 1;
    }

    /* write default values to file */
    fprintf(file, 
        "start_on_login = %d\n"
        "start_in_tray = %d\n"
        "minimize_on_exit = %d\n",
        start_on_login,
        start_in_tray,
        minimize_on_exit );

    /* forces the contents of the buffer to be written to file */
    fflush(file);
    fclose(file);
    
    fprintf(stderr, "Saved config file %S\n", filePath);
    return 0;
}