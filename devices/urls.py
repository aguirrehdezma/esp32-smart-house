from rest_framework import routers
from .viewsets import SensorDataViewSet


route = routers.SimpleRouter()
route.register(r'sensor_data', SensorDataViewSet)
urlpatterns = route.urls
