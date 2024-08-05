FetchContent_Declare(modernsqlite
    GIT_REPOSITORY https://github.com/SqliteModernCpp/sqlite_modern_cpp
    GIT_TAG dev
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    UPDATE_COMMAND ""
)

FetchContent_GetProperties(modernsqlite)
if(NOT modernsqlite_POPULATED)
  FetchContent_Populate(modernsqlite)
endif()