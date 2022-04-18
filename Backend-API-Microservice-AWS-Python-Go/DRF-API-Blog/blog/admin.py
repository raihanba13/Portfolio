from csv import list_dialects
from pyexpat import model
from django.contrib import admin
from . import models 

@admin.register(models.Post)
class AuthorAdmin(admin.ModelAdmin):
    list_displa = ('title', 'id', 'status', 'slug', 'author')
    prepopulated_fields = {'slug': ('title',),}

admin.site.register(models.Category)
