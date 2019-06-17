from django.urls import path
from todoapp.views import *

urlpatterns = [
    # path('college/', clg_name, name='clg_name'),
    # path('college_list/', clg_names_and_acronyms),
    # path('college_students/<int:id>', clg_students),

    path('', lambda ref: redirect('login', permanent=True)),

    path(r'todolist', ToDoListView.as_view()),

    path(r'login/', LoginController.as_view(), name='login'),
    path(r'signup/', SignUpController.as_view(), name='signup'),
    path(r'logout/', logout_user, name='logout'),

    # Auth Token
    path(r'api/v1/auth-token', CustomAuthToken.as_view(), name='api_auth_token'),
]

#TODO: CHECK THIS OUT
# {{ student_form.dropped_out.id_for_label }}