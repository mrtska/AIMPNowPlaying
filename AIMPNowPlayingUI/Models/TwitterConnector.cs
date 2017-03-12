using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Livet;

using CoreTweet;
using CoreTweet.Core;
using static CoreTweet.OAuth;
using AIMPNowPlayingUI.ViewModels;

namespace AIMPNowPlayingUI.Models {
    public class TwitterConnector : NotificationObject {


        private OAuthSession RequestSession;


        private Tokens Token;

        private MainWindowViewModel Owner;

        public TwitterConnector(MainWindowViewModel vm) {

            Owner = vm;
            
        }

        public async void RequestToken() {

            RequestSession = await AuthorizeAsync(TwitterConstants.ConsumerKey, TwitterConstants.ConsumerSecret);
            System.Diagnostics.Process.Start(RequestSession.AuthorizeUri.OriginalString);
        }

        public async void Verify(string pin) {

            if(RequestSession != null) {

                Token = await RequestSession.GetTokensAsync(pin);
                Owner.AccessToken = Token.AccessToken;
                Owner.AccessTokenSecret = Token.AccessTokenSecret;
                Owner.CanTweet = true;
            }
        }


        public async void Tweet(string text) {

            await Token.Statuses.UpdateAsync(text);
        }







    }
}
