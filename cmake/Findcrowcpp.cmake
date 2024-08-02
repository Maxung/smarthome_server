include(FetchContent)
FetchContent_Declare(
    crowcpp
    GIT_REPOSITORY https://github.com/CrowCpp/Crow
    GIT_TAG master
    GIT_SHALLOW YES
    UPDATE_COMMAND ""
)

FetchContent_MakeAvailable(crowcpp)

set(crowcpp_FOUND 1)