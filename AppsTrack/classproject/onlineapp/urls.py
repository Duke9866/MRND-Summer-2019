from django.urls import path
from onlineapp.views import *
from onlineapp import views_backup

urlpatterns = [
    # path('college/', clg_name, name='clg_name'),
    # path('college_list/', clg_names_and_acronyms),
    # path('college_students/<int:id>', clg_students),

    path('', lambda ref: redirect('login', permanent=True)),

    path(r'test_path', views_backup.test_view, name='test_path'),

    path(r'colleges/', CollegeView.as_view(), name='colleges'),
    path(r'colleges/<int:pk>', CollegeView.as_view(), name='college_id'),
    path(r'colleges/<str:acronym>', CollegeView.as_view(), name='college_acronym'),

    path(r'college/add', AddCollegeView.as_view(), name='add_college'),
    path(r'colleges/<int:pk>/edit', AddCollegeView.as_view(), name='edit_college'),
    path(r'colleges/<int:pk>/delete', AddCollegeView.as_view(), name='delete_college'),

    path(r'colleges/<int:pk1>/student/add', AddStudentView.as_view(), name='add_student'),
    path(r'colleges/<int:pk1>/student/<int:pk2>/edit', AddStudentView.as_view(), name='edit_student'),
    path(r'colleges/<int:pk1>/student/<int:pk2>/delete', AddStudentView.as_view(), name='delete_student'),

    path(r'login/', LoginController.as_view(), name='login'),
    path(r'signup/', SignUpController.as_view(), name='signup'),
    path(r'logout/', logout_user, name='logout'),

    # API paths
    # path('api/v1/colleges', ApiCollegeView, name='api_colleges'),
    # path('api/v1/colleges/<int:pk>', ApiCollegeView, name='api_college_id'),

    path(r'api/v1/colleges', ApiCollegeView.as_view(), name='api_colleges'),
    path(r'api/v1/colleges/<int:pk>', ApiCollegeView.as_view(), name='api_college_id'),

    path(r'api/v1/colleges/add', ApiCollegeView.as_view(), name='api_add_college'),
    path(r'api/v1/colleges/<int:pk>/edit', ApiCollegeView.as_view(), name='api_edit_college'),
    path(r'api/v1/colleges/<int:pk>/delete', ApiCollegeView.as_view(), name='api_delete_college'),

    path(r'api/v1/colleges/<int:pk>/student/add', ApiStudentView.as_view(), name='api_add_student'),
    path(r'api/v1/colleges/<int:pk1>/student/<int:pk2>/edit', ApiCollegeView.as_view(), name='api_edit_college'),
    path(r'api/v1/colleges/<int:pk1>/student/<int:pk2>/delete', ApiCollegeView.as_view(), name='api_delete_college'),

    # Auth Token
    path(r'api/v1/auth-token', CustomAuthToken.as_view(), name='api_auth_token'),
]

#TODO: CHECK THIS OUT
# {{ student_form.dropped_out.id_for_label }}