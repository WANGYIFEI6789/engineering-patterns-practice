# 测试配置
add_executable(test_mvp
    test_mvp.cpp
    ../src/model/vehicle_model.cpp
    ../src/view/dashboard_view.cpp
    ../src/present/main_presenter.cpp
)

target_include_directories(test_mvp PRIVATE ../include)
target_link_libraries(test_mvp Threads::Threads)

add_test(NAME MVPTest COMMAND test_mvp)