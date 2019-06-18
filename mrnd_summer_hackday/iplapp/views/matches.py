from django.views import View
from django.shortcuts import render, redirect, get_object_or_404
from django.contrib.auth.mixins import LoginRequiredMixin
from django.db.models import Sum, Count
from iplapp.models import *

class MatchView(LoginRequiredMixin, View):
    login_url = 'login'

    def get(self, request, *args, **kwargs):
        permissions = request.user.get_all_permissions()
        seasons = Season.objects.all().order_by('-season')
        current_user = request.user

        if kwargs:
            season = Season.objects.get(season=kwargs.get('season'))
            match = Match.objects.get(id=kwargs.get('match_id'))
            deliveries = Delivery.objects.filter(match=kwargs.get('match_id')).filter(inning=kwargs.get('inning'))
            inning = kwargs.get('inning')

            top_batsman = Delivery.objects.values('batsman', 'batting_team').filter(match=kwargs['match_id']).annotate(
                total_runs=Sum('total_runs')).order_by('-total_runs')[:3]
            top_bowler = Delivery.objects.values('bowler', 'bowling_team').filter(match=kwargs['match_id']).annotate(
                total_wickets=Count('player_dismissed')).order_by('-total_wickets')[:3]

        return render(
            request,
            template_name='match.html',
            context={
                'user': current_user,
                'year': season,
                'seasons': seasons,
                'inning': inning,
                'match': match,
                'top_batsman': top_batsman,
                'top_bowler': top_bowler,
                'deliveries': enumerate(deliveries, start=1),
                'title': 'IPL Clone App',
                'permissions': permissions,
            }
        )