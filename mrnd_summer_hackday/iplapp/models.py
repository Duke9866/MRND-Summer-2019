from django.db import models

# Create your models here.
class Season(models.Model):
    season = models.IntegerField()

class Match(models.Model):
    id = models.IntegerField(primary_key=True)
    city = models.CharField(max_length=64)
    date = models.DateField(null=True)
    team1 = models.CharField(max_length=64)
    team2 = models.CharField(max_length=64)
    toss_winner = models.CharField(max_length=64)
    toss_decision = models.CharField(max_length=64)
    result = models.CharField(max_length=64)
    dl_applied = models.IntegerField()
    winner = models.CharField(max_length=64)
    win_by_runs = models.IntegerField()
    win_by_wickets = models.IntegerField()
    player_of_match = models.CharField(max_length=64)
    venue = models.CharField(max_length=64)
    umpire1 = models.CharField(max_length=64, null=True)
    umpire2 = models.CharField(max_length=64, null=True)
    umpire3 = models.CharField(max_length=64, null=True)

    season = models.ForeignKey(Season, on_delete=models.CASCADE)

class Delivery(models.Model):
    inning = models.IntegerField()
    batting_team = models.CharField(max_length=64)
    bowling_team = models.CharField(max_length=64)
    over = models.IntegerField()
    ball = models.IntegerField()
    batsman = models.CharField(max_length=64)
    non_striker = models.CharField(max_length=64)
    bowler = models.CharField(max_length=64)
    is_super_over = models.BooleanField()
    wide_runs = models.IntegerField()
    bye_runs = models.IntegerField()
    legbye_runs = models.IntegerField()
    noball_runs = models.IntegerField()
    penalty_runs = models.IntegerField()
    batsman_runs = models.IntegerField()
    extra_runs = models.IntegerField()
    total_runs = models.IntegerField()
    player_dismissed = models.CharField(max_length=64, null=True)
    dismissal_kind = models.CharField(max_length=64, null=True)
    fielder = models.CharField(max_length=64, null=True)

    match = models.ForeignKey(Match, on_delete=models.CASCADE)




