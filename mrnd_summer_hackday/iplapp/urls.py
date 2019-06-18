from django.shortcuts import redirect
from django.urls import path
from iplapp.views import *
from iplapp.forms import *

urlpatterns = [
    path('login/', LoginController.as_view(), name='login'),
    path('signup/', SignUpController.as_view(), name='signup'),
    path('logout/', logout_user, name='logout'),

    path('seasons/', lambda x: redirect('seasons/2019/1', permanent=True)),
    path('seasons/<int:season>/<int:page>/', SeasonView.as_view(), name='seasons'),
    path('seasons/<int:season>/match/<int:match_id>/inning/<int:inning>/', MatchView.as_view(), name='match'),

    path('points/<int:season>/', PointsView.as_view(), name='points'),
]