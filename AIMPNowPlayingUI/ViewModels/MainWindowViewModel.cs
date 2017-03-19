using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;

using Livet;
using Livet.Commands;
using Livet.Messaging;
using Livet.Messaging.IO;
using Livet.EventListeners;
using Livet.Messaging.Windows;


using AIMPNowPlayingUI.Models;

namespace AIMPNowPlayingUI.ViewModels {
    public class MainWindowViewModel : ViewModel {



        #region PinCode変更通知プロパティ
        private string _PinCode;

        public string PinCode {
            get { return _PinCode; }
            set { 
                if (_PinCode == value)
                    return;
                _PinCode = value;
                RaisePropertyChanged();
            }
        }
        #endregion

        public string AccessToken;
        public string AccessTokenSecret;


        private TwitterConnector Connector;

        public bool CanTweet = false;


        public MainWindowViewModel() {

            Connector = new TwitterConnector(this);
        }

        public void RequestToken() {

            Connector.RequestToken();
        }

        public void Verify() {

            Connector.Verify(PinCode);
        }


        public void Tweet(string text, string jacketPath) {

            Connector.Tweet(text, jacketPath);
        }


    }
}
